#include <istream>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <tuple>
#include <unordered_set>
#include <algorithm>
#include <assert.h>
#include <set>
#include <unordered_map>
// Nota: il grafo è connesso

using namespace std;

# define UNKNOWN -1

class Node;

ostream & operator<<(ostream & os, Node* node);
ostream & operator<<(ostream & os, Node& node);
ostream & operator<<(ostream & os, list<Node*>& path);
template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);
template<typename T>
ostream & operator<<(ostream & os, list<T>& v);

class Link{
public:
    int cost;
    Node* node;

    Link(Node* _node, int _const){
        node = _node;
        cost = _const;
    }
};

class Node{
public:
    int value;
    vector<Link> adjacent_nodes;
    int distance_from_powarts;
    vector<int> choke_points;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Link>();
        distance_from_powarts = -1;
        choke_points = vector<int>();
    }

    void insert_adjacent_node(Node* node_to_insert, int cost) {
        adjacent_nodes.emplace_back(node_to_insert,cost);
    }

    // when this function is called all nodes must have distance_from_powarts
    // already initialized, and the nodes with a distance lower that the one on the
    // current nodes must have choke_points already initialized;
    void calculate_choke_points(){
        assert(distance_from_powarts != -1);
        list<int> choke_points_temp;
        bool is_first = true;
        for(auto adjacent_node: adjacent_nodes){
            // the cost to reach powarts using this link.
            int cost = adjacent_node.node->distance_from_powarts + adjacent_node.cost;
            // this must be true
            assert(cost >= distance_from_powarts);
            assert(adjacent_node.node->distance_from_powarts != -1);
            assert(adjacent_node.cost != 0);
            // if is not on the minimum path, no need to check
            if(cost > distance_from_powarts){
                continue;
            }

            if(is_first){
                choke_points_temp = list<int>(adjacent_node.node->choke_points.begin(),adjacent_node.node->choke_points.end());
                is_first = false;
            }else{
                // make set intersection between choke_points_temp and adjacent_node's choke points
                list<int> intersection = list<int>();

                set_intersection(choke_points_temp.begin(),choke_points_temp.end(),
                                 adjacent_node.node->choke_points.begin(),adjacent_node.node->choke_points.end(),
                                 back_inserter(intersection));

                choke_points_temp = std::move(intersection);
            }
        }
        if(distance_from_powarts!=0){
            choke_points_temp.push_back(value);
        }
        choke_points = vector<int>(choke_points_temp.begin(),choke_points_temp.end());
    }

};



class Graph{
public:
    vector<Node> nodes;

    Graph(int number_of_nodes){
        nodes = vector<Node>();
        nodes.reserve(number_of_nodes);
        for(int i=0; i<number_of_nodes; i++){
            nodes.push_back(Node(i));
        }
    }

    void insert_edge(int from, int to, int cost){
        nodes[from].insert_adjacent_node(&nodes[to],cost);
    }

    void insert_double_edge(int n1, int n2, int cost){
        insert_edge(n1,n2, cost);
        insert_edge(n2,n1, cost);
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

vector<Node*> propagate_distance(Graph& graph, Node* start_node){

    vector<Node*> order_vector = vector<Node*>();
    order_vector.reserve(graph.nodes.size());

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
        if(next_node -> distance_from_powarts == -1){
            next_node -> distance_from_powarts = current_distance;
        }else{
            continue;
        }

        order_vector.push_back(next_node);

        for(auto adjacent_node: next_node->adjacent_nodes) {
            auto new_node = adjacent_node.node;
            auto new_distance = adjacent_node.cost + current_distance;
            distance_to_consider.insert_new_distance(new_distance);
            distance_to_queue.insert_node(new_node, new_distance);
        }
    }

    return order_vector;
}

int main(){
    int n_nodes;
    int n_edges;
    int powarts;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_edges;
    input >> powarts;


    Graph graph(n_nodes);

    for(int i=0; i<n_edges; i++){
        int n1,n2,cost;
        input >> n1;
        input >> n2;
        input >> cost;
        graph.insert_double_edge(n1,n2,cost);
    }

    vector<Node*> order_vector = propagate_distance(graph,&graph.nodes[powarts]);

    for(Node* node: order_vector){
        node->calculate_choke_points();
    }

    // count choke points;
    vector<int> choke_points = vector<int>();
    choke_points.reserve(graph.nodes.size());
    for(int i=0; i<graph.nodes.size(); i++){
        choke_points.push_back(0);
    }

    cout << order_vector;

    for(Node* node: order_vector){
        for(int choke_point: node->choke_points){
            choke_points[choke_point]++;
        }
    }

    int city_max_damage;
    int max_city_choked = 0;

    for(int i=0; i<graph.nodes.size(); i++){
        if(choke_points[i] > max_city_choked){
            max_city_choked = choke_points[i];
            city_max_damage = i;
        }
    }

    vector<int> list_city_choked = vector<int>();
    list_city_choked.reserve(max_city_choked);

    for(auto node: graph.nodes){
        bool is_choked = (find(node.choke_points.begin(), node.choke_points.end(),
                               city_max_damage) != node.choke_points.end());
        if(is_choked){
            list_city_choked.push_back(node.value);
        }
    }

    cout << "The max num of cities blocked is: " << max_city_choked << endl;

    cout << graph.nodes;

    output << max_city_choked << endl;
    for(int city: list_city_choked){
        output << city << endl;
    }

    output.close();
    input.close();
}


ostream & operator<<(ostream & os, list<Node*>& path){
    os << "[";
    for(auto node: path){
        os << " -> " << node->value;
    }
    os << "]";
    return os;
}

ostream & operator<<(ostream & os, Node& node){
    os << "{id:  " << node.value <<", distance: " << node.distance_from_powarts << ", cp: [";
    for(auto cp: node.choke_points){
        os << cp << ", ";
    }
    os <<"]}";
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

template<typename T>
ostream & operator<<(ostream & os, list<T>& v){
    os << "[" << endl;
    int c = 0;
    for(auto node: v){
        os << "\t" << c <<": " << node << "\n";
        c++;
    }
    os << "]\n";
    return os;
}
