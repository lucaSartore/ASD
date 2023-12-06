#include <iostream>
#include <fstream>
#include <vector>
#include <execution>
#include <queue>
#include <unordered_map>
#include <set>
using namespace std;

typedef enum {FAB_LAB, STUDENTS, IMPOSTOR, NONE} NodeType;

class Node;
class Link;
class LinkPtr{
public:
    vector<Link>* v;
    int index;
    LinkPtr(vector<Link>* _v, int _index){
        v = _v;
        index = _index;
    }
    Link* operator->() const{
        return &(*v)[index];
    }
};

ostream & operator<<(ostream & os, Node& node);
ostream & operator<<(ostream & os, Node* node);
template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);

class Link{
public:
    Node* to;
    int cost_min;
    int cost_max;
    int current_cost;

    bool locked;

    Link(Node* to, int cost_min, int cost_max){
        this->to = to;
        this->cost_min = cost_min;
        this->cost_max = cost_max;
        this->current_cost = cost_min;
        locked = false;
    }

    void set_to_max(){
        if(locked)return;
        current_cost = cost_max;
    }
    void set_to_min(){
        if(locked)return;
        current_cost = cost_min;
    }
};

class LinkReversed{
public:
    Node* to;
    LinkPtr original_link;
    LinkReversed(Node *to, LinkPtr original_link, LinkPtr originalLink) : original_link(originalLink) {
        this->to = to;
        this->original_link = original_link;
    }
};

class Node{
public:
    NodeType node_type;
    int value;
    vector<Link> adjacent_nodes;
    vector<LinkReversed> adjacent_nodes_reversed;
    int max_distance_fab_lab;
    int min_distance_fab_lab;
    Node(int n){
        value = n;
        node_type = NONE;
        adjacent_nodes = vector<Link>();
        adjacent_nodes_reversed = vector<LinkReversed>();
        max_distance_fab_lab = -1;
        min_distance_fab_lab = -1;
    }

    LinkPtr insert_adjacent_node(Link adjacent){
        adjacent_nodes.push_back(adjacent);
        return LinkPtr(&adjacent_nodes,adjacent_nodes.size()-1);
    }

    void insert_adjacent_node_reversed(LinkReversed adjacent){
        adjacent_nodes_reversed.push_back(adjacent);
    }

};
class DistancesToConsider{
private:
    // set is a sorted list!!! (not an hash set)
    set<int> distance_to_consider;
public:
    DistancesToConsider(){
        distance_to_consider = set<int>();
    }
    void insert_new_distance(int distance){
        // insert the distance, and keep the list sort
        distance_to_consider.insert(distance);
    }
    int next_distance_to_consider(){
        // get the first element in the set
        auto begin = distance_to_consider.begin();
        if(begin == distance_to_consider.end()){
            return -1;
        }
        int to_return = *begin;
        // remove the element from the set
        distance_to_consider.erase(begin);
        return to_return;
    }
};

class DistanceToQueue{
private:
    unordered_map<int,queue<Node*>> distance_to_queue;
public:
    DistanceToQueue(){
        distance_to_queue = unordered_map<int,queue<Node*>>();
    }
    void insert_node(Node* node, int distance){
        if(distance_to_queue.find(distance) == distance_to_queue.end()){
            distance_to_queue[distance] = queue<Node*>();
        }
        distance_to_queue[distance].push(node);
    }
    Node* get_node(int distance){
        if(distance_to_queue.find(distance) == distance_to_queue.end()){
            return nullptr;
        }
        if(distance_to_queue[distance].empty()){
            return nullptr;
        }
        Node* node = distance_to_queue[distance].front();
        distance_to_queue[distance].pop();
        return node;
    }
};

class Graph{
public:
    vector<Node> nodes;
    Node* fab_lab;
    Node* students;
    Node* impostor;

    vector<LinkPtr> links;

    Graph(int n_nodes, int pos_impostor, int pos_students, int pos_fab_lab){
        nodes = vector<Node>();
        nodes.reserve(n_nodes);
        for(int i=0; i<n_nodes; i++){
            nodes.emplace_back(i);
        }
        nodes[pos_impostor].node_type = IMPOSTOR;
        nodes[pos_students].node_type = STUDENTS;
        nodes[pos_fab_lab].node_type = FAB_LAB;

        fab_lab = &nodes[pos_fab_lab];
        students = &nodes[pos_students];
        impostor = &nodes[pos_impostor];

        links = vector<LinkPtr>();
    }

    void insert_link(int from, int to, int cost_min, int cost_max){
        LinkPtr link = nodes[from].insert_adjacent_node(Link(&nodes[to],cost_min,cost_max));
        nodes[to].insert_adjacent_node_reversed(LinkReversed(&nodes[from], link, LinkPtr(nullptr, 0)));
        links.push_back(link);
    }


    vector<int> distance_to_node(int start_node_index){

        Node* start_node = &nodes[start_node_index];

        vector<int> distances = vector<int>(nodes.size(),-1);

        auto distance_to_consider = DistancesToConsider();
        auto distance_to_queue = DistanceToQueue();

        distance_to_consider.insert_new_distance(0);
        distance_to_queue.insert_node(start_node,0);


        int current_distance = distance_to_consider.next_distance_to_consider();

        while(true){
            if(current_distance == -1){
                break;
            }

            Node* next_node = distance_to_queue.get_node(current_distance);
            if(next_node == nullptr){
                current_distance = distance_to_consider.next_distance_to_consider();
                continue;
            }

            // the same node may be in the queue twice, since the first time is discovered
            // is not always the path with the shortest distance
            if(distances[next_node->value] == -1){
                distances[next_node->value] = current_distance;
            }else{
                continue;
            }


            for(auto& adjacent_node: next_node->adjacent_nodes_reversed) {

                auto new_node = adjacent_node.to;
                auto new_distance = adjacent_node.original_link->current_cost + current_distance;
                distance_to_consider.insert_new_distance(new_distance);
                distance_to_queue.insert_node(new_node, new_distance);
            }
        }

        return distances;
    }

    void set_max_and_min_distances(){

        for(auto link: links){
            link->set_to_max();
        }

        vector<int> distances_from_fab_lab = distance_to_node(fab_lab->value);
        for(int i=0; i<nodes.size(); i++){
            nodes[i].max_distance_fab_lab = distances_from_fab_lab[i];
        }

        for(auto link: links){
            link->set_to_min();
        }

        vector<int> distances_to_fab_lab = distance_to_node(fab_lab->value);
        for(int i=0; i<nodes.size(); i++){
            nodes[i].min_distance_fab_lab = distances_to_fab_lab[i];
        }

    }

};

int main(){

    int n_nodes, n_links, n_special_links;
    int pos_impostor, pos_students, pos_fab_lab;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes >> n_links >> n_special_links;
    input >> pos_impostor >> pos_students >> pos_fab_lab;

    Graph graph = Graph(n_nodes,pos_impostor,pos_students,pos_fab_lab);

    for(int i=0; i<n_links; i++){
        int from,to,cost;
        input >> from >> to >> cost;
        //if(from == pos_fab_lab)continue;
        graph.insert_link(from,to,cost,cost);
    }

    for(int i=0; i<n_special_links; i++){
        int from,to,cost_min,cost_max;
        input >> from >> to >> cost_min >> cost_max;
        //if(from == pos_fab_lab)continue;
        graph.insert_link(from,to,cost_min,cost_max);
    }

    graph.set_max_and_min_distances();

    cout << graph.nodes;

    output.close();
    input.close();
}




ostream & operator<<(ostream & os, Node& node){
    os << "{id:  " << node.value <<", distance_min: " << node.min_distance_fab_lab << ", distance_max: " << node.max_distance_fab_lab << "}";
    return os;
}

ostream & operator<<(ostream & os, Node* node){
    os << *node;
    return os;
}


template<typename T>
ostream & operator<<(ostream & os, vector<T>& v){
    os << "[" << endl;
    int c = 0;
    for(auto node: v){
        os << "\t" << c <<": " << node << "\n";
        c++;
    }
    os << "]\n";
    return os;
}
