#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include <cassert>
using namespace std;

typedef enum {FAB_LAB, STUDENTS, IMPOSTOR, NONE} NodeType;
typedef enum {BY_NONE=0, BY_IMPOSTOR=1, BY_STUDENTS=2, BY_BOTH=3} ReachableByType;
typedef enum {DRAW=0, IMPOSTOR_WIN=1, STUDENTS_WIN=2} ResultType;
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
ostream & operator<<(ostream & os, ResultType& r);
ostream  & operator<<(ostream & os, ReachableByType& reachable_by_type);
ostream  & operator<<(ostream & os, NodeType& node_type);
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

    void lock(){
        locked = true;
    }

    void set_to_max(){
        if(locked)return;
        current_cost = cost_max;
    }
    void set_to_min(){
        if(locked)return;
        current_cost = cost_min;
    }

    void reduce_cost(int amount){
        if(locked)return;
        current_cost -= amount;
        if(current_cost < cost_min){
            current_cost = cost_min;
        }
        if(current_cost > cost_max){
            current_cost = cost_max;
        }
    }

    bool was_settable()const{
        return cost_max != cost_min;
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
    int distance_students_to_here;
    int current_distance_to_fab_lab;
    int distance_impostor_to_here;
    ReachableByType reachable_by;
    Node(int n){
        value = n;
        node_type = NONE;
        adjacent_nodes = vector<Link>();
        adjacent_nodes_reversed = vector<LinkReversed>();
        max_distance_fab_lab = -1;
        min_distance_fab_lab = -1;
        distance_students_to_here = -1;
        distance_impostor_to_here = -1;
        current_distance_to_fab_lab = -1;
        reachable_by = BY_NONE;
    }

    LinkPtr insert_adjacent_node(Link adjacent){
        adjacent_nodes.push_back(adjacent);
        return LinkPtr(&adjacent_nodes,adjacent_nodes.size()-1);
    }

    void insert_adjacent_node_reversed(LinkReversed adjacent){
        adjacent_nodes_reversed.push_back(adjacent);
    }

    void set_reachable_by(ReachableByType _reachable_by){
        reachable_by = (ReachableByType) (reachable_by | _reachable_by);
    }
    bool is_reachable_by(ReachableByType _reachable_by)const{
        return (reachable_by & _reachable_by) == _reachable_by;
    }

    void propagate_reachable_by(){
        for(auto& adjacent_node: adjacent_nodes){
            if(adjacent_node.to->is_reachable_by(reachable_by)){
                continue;
            }
            adjacent_node.to->set_reachable_by(reachable_by);
            adjacent_node.to->propagate_reachable_by();
        }
    }

    inline void try_reduce_distance_fab_lab(int new_distance){
        if(new_distance < current_distance_to_fab_lab){
            current_distance_to_fab_lab = new_distance;
        }
    }

    void update_distance_from_fab_lab(){
        for(auto adj: adjacent_nodes){
            if(adj.to->current_distance_to_fab_lab == -1){
                continue;
            }
            try_reduce_distance_fab_lab(adj.to->current_distance_to_fab_lab+adj.current_cost);
        }
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

        nodes[pos_impostor].set_reachable_by(BY_IMPOSTOR);
        nodes[pos_students].set_reachable_by(BY_STUDENTS);

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

    vector<int> distance_from_node(int start_node_index){

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


            for(auto& adjacent_node: next_node->adjacent_nodes) {

                auto new_node = adjacent_node.to;
                auto new_distance = adjacent_node.current_cost + current_distance;
                distance_to_consider.insert_new_distance(new_distance);
                distance_to_queue.insert_node(new_node, new_distance);
            }
        }

        return distances;
    }

    void fill_distance_students_to_here(){
        vector<int> distances_from_students = distance_from_node(students->value);
        for(int i=0; i<nodes.size(); i++){
            nodes[i].distance_students_to_here = distances_from_students[i];
        }
    }

    void fill_distance_impostor_to_here(){
        vector<int> distances_from_students = distance_from_node(impostor->value);
        for(int i=0; i<nodes.size(); i++){
            nodes[i].distance_impostor_to_here = distances_from_students[i];
        }
    }


    void set_links_to_max(){
        for(auto link: links){
            link->set_to_max();
        }
    }

    void set_links_to_min(){
        for(auto link: links){
            link->set_to_min();
        }
    }

    void set_max_and_min_distances(){

        set_links_to_max();

        vector<int> distances_from_fab_lab = distance_to_node(fab_lab->value);
        for(int i=0; i<nodes.size(); i++){
            nodes[i].max_distance_fab_lab = distances_from_fab_lab[i];
        }

        set_links_to_min();

        vector<int> distances_to_fab_lab = distance_to_node(fab_lab->value);
        for(int i=0; i<nodes.size(); i++){
            nodes[i].min_distance_fab_lab = distances_to_fab_lab[i];
        }

    }

    void propagate_reachable_by(){
        impostor->propagate_reachable_by();
        students->propagate_reachable_by();
    }

    void lock_easy_links(){
        for(auto& node: nodes){
            if(node.reachable_by == BY_STUDENTS){
                for(auto& link: node.adjacent_nodes){
                    link.set_to_max();
                    link.lock();
                }
            }else if(node.reachable_by == BY_IMPOSTOR){
                for(auto& link: node.adjacent_nodes){
                    link.set_to_min();
                    link.lock();
                }
            }
        }
    }

    // this function locks the links that are nor straightforward to kind a value, as they can be used
    // by both impostor and students
    void lock_hard_links(){
        set_links_to_max();
        fill_distance_impostor_to_here();
        fill_distance_students_to_here();
        fill_current_distance_from_fab_lab();


        //cout << this->nodes;

        Node* start_node = fab_lab;

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

            next_node->update_distance_from_fab_lab();

            for(auto& adjacent_node_link: next_node->adjacent_nodes_reversed) {
                auto adjacent_node = adjacent_node_link.to;

                if(adjacent_node->reachable_by != BY_BOTH){
                    continue;
                }
                if(adjacent_node->current_distance_to_fab_lab < next_node->current_distance_to_fab_lab){
                    continue;
                }

                adjacent_node->try_reduce_distance_fab_lab(next_node->current_distance_to_fab_lab + adjacent_node_link.original_link->current_cost);

                // I try to reduce the distance of the fom the fab lab, as long as i don't advantage the students
                int margin = next_node->current_distance_to_fab_lab + adjacent_node_link.original_link->current_cost
                        + adjacent_node->distance_students_to_here - students->max_distance_fab_lab;

                if(adjacent_node->distance_students_to_here == -1){
                    margin = 10000000;
                }
                //assert(margin>=0);
                adjacent_node_link.original_link->reduce_cost(margin);
                adjacent_node_link.original_link->lock();

                // update the distance... (if the node has already a faster path, id dose not update it
                adjacent_node->try_reduce_distance_fab_lab(
                        next_node->current_distance_to_fab_lab +
                        adjacent_node_link.original_link->current_cost);

                auto new_distance = adjacent_node->current_distance_to_fab_lab;

                if(new_distance <= current_distance){
                    continue;
                }

                distance_to_consider.insert_new_distance(new_distance);

                distance_to_queue.insert_node(adjacent_node, new_distance);
            }
        }
    }

    ResultType get_result(){
        fill_current_distance_from_fab_lab();
        if(students->current_distance_to_fab_lab < impostor->current_distance_to_fab_lab){
            return STUDENTS_WIN;
        }else if(students->current_distance_to_fab_lab > impostor->current_distance_to_fab_lab){
            return IMPOSTOR_WIN;
        }else{
            return DRAW;
        }
    }

    void fill_current_distance_from_fab_lab(){
        vector<int> distances_from_fab_lab = distance_to_node(fab_lab->value);
        for(int i=0; i<nodes.size(); i++){
            nodes[i].current_distance_to_fab_lab = distances_from_fab_lab[i];
        }
    }

    vector<int> best_impostor_path()const{
        vector<int> to_return = vector<int>();
        to_return.push_back(impostor->value);
        Node* current_node = impostor;
        while (current_node!=fab_lab){
            for(auto& adj: current_node->adjacent_nodes){
                if(adj.to->current_distance_to_fab_lab == -1){
                    continue;
                }
                if(adj.to->current_distance_to_fab_lab == current_node->current_distance_to_fab_lab - adj.current_cost){
                    current_node = adj.to;
                    break;
                }
            }
            to_return.push_back(current_node->value);
        }
        return to_return;
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
    graph.propagate_reachable_by();
    graph.lock_easy_links();
    graph.lock_hard_links();
    ResultType result = graph.get_result();
    //cout << graph.nodes << endl;
    cout << result << endl;
    cout << "time impostor: " << graph.impostor->current_distance_to_fab_lab << endl;
    cout << "time students:  " << graph.students->current_distance_to_fab_lab << endl;

    output << (int)result << endl;
    output << graph.impostor->current_distance_to_fab_lab << " " << graph.students->current_distance_to_fab_lab << endl;

    for(auto& link: graph.links){
        if(link->was_settable()){
            output << link->current_cost << " ";
        }
    }
    output << endl;

    auto best_path = graph.best_impostor_path();

    output << best_path.size() << endl;
    for(int room: best_path){
        output << room << " ";
    }

    output.close();
    input.close();
}

ostream & operator<<(ostream & os, ResultType& r){
    switch(r){
        case DRAW:
            os << "DRAW";
            break;
        case IMPOSTOR_WIN:
            os << "IMPOSTOR";
            break;
        case STUDENTS_WIN:
            os << "STUDENTS";
            break;
    }
    return os;
}
ostream  & operator<<(ostream & os, NodeType& node_type){
    switch(node_type){
        case FAB_LAB:
            os << "FAB_LAB";
            break;
        case STUDENTS:
            os << "STUDENTS";
            break;
        case IMPOSTOR:
            os << "IMPOSTOR";
            break;
        case NONE:
            os << "NONE";
            break;
    }
    return os;
}

ostream  & operator<<(ostream & os, ReachableByType& reachable_by_type){
    switch(reachable_by_type){
        case BY_NONE:
            os << "BY_NONE";
            break;
        case BY_IMPOSTOR:
            os << "BY_IMPOSTOR";
            break;
        case BY_STUDENTS:
            os << "BY_STUDENTS";
            break;
        case BY_BOTH:
            os << "BY_BOTH";
            break;
    }
    return os;
}

ostream & operator<<(ostream & os, Node& node){
    os << "{id:  " << node.value <<
    //", distance_min: " << node.min_distance_fab_lab <<
    //", distance_max: " << node.max_distance_fab_lab <<
    //" reachable_by: " << node.reachable_by <<
    ", distance_students_to_here: " << node.distance_students_to_here << "}" <<
       ", distance_to_fab_lab: " << node.current_distance_to_fab_lab << "}";
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
