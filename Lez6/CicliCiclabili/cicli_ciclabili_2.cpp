#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <queue>
#include <algorithm>
using namespace std;


class Node;
class GroupNode;

ostream & operator<<(ostream & os, Node* node);
ostream & operator<<(ostream & os, Node& node);
template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);
ostream & operator<<(ostream & os, GroupNode* node);
ostream & operator<<(ostream & os, GroupNode& node);


int current_time;
class Node{
public:
    int value;
    int discovery_time;
    int finish_time;
    vector<Node*> adjacent_nodes;
    int distance_from_root;
    Node* father_intersection;
    bool is_root;
    Node(int n){
        value = n;
        discovery_time = -1;
        finish_time = -1;
        adjacent_nodes = vector<Node*>();
        distance_from_root = -1;
        father_intersection = nullptr;
        is_root = false;
    }

    void insert_adjacent_node(Node* adjacent){
        adjacent_nodes.push_back(adjacent);
    }

    // this node is an itersection if it hase more that 3 adjacent nodes
    bool is_intersection() {
        return adjacent_nodes.size() >= 3 || (is_root && adjacent_nodes.size() >= 2);
    }

    void dept_first_visit() {
        discovery_time = current_time;
        current_time++;

        // do the one on the same loop first!
        for (auto adjacent: adjacent_nodes) {
            if (adjacent->discovery_time != -1) {
                continue;
            }
            if(adjacent->distance_from_root > distance_from_root){
                continue;
            }
            adjacent->dept_first_visit();
        }

        for (auto adjacent: adjacent_nodes) {
            if (adjacent->discovery_time != -1) {
                continue;
            }
            adjacent->dept_first_visit();
        }
        finish_time = current_time;
        current_time++;
    }

    void propagate_father_intersection(Node* new_father_intersection) {
        father_intersection = new_father_intersection;

        if (is_intersection()) {
            new_father_intersection = this;
        }

        for (auto adjacent: adjacent_nodes) {
            // skip the father
            if (adjacent->discovery_time < discovery_time) {
                continue;
            }
            adjacent->propagate_father_intersection(new_father_intersection);
        }
    }

    bool is_ancestor_of(Node* const tested_child) const{
        return discovery_time <= tested_child->discovery_time && finish_time >= tested_child->discovery_time;
    }
};


class Graph{
public:
    vector<Node> nodes;
    int root;

    Graph(int n_nodes, int _root = 0){
        nodes = vector<Node>();
        root = _root;
        nodes.reserve(n_nodes);
        for(int i=0; i<n_nodes; i++){
            nodes.emplace_back(i);
        }
    }

    void insert_link(int n1, int n2){
        nodes[n1].insert_adjacent_node(&nodes[n2]);
        nodes[n2].insert_adjacent_node(&nodes[n1]);
    }

    void propagate_distance_from_root(){
        nodes[root].distance_from_root = 0;

        queue<Node*> to_visit = queue<Node*>();
        to_visit.push(&nodes[root]);

        while (!to_visit.empty()){

            Node* node = to_visit.front();
            to_visit.pop();
            for(auto adjacent: node->adjacent_nodes){
                if(adjacent->distance_from_root != -1){
                    continue;
                }
                adjacent->distance_from_root = node->distance_from_root + 1;
                to_visit.push(adjacent);
            }
        }
    }

    void insert_times(){
        current_time = 0;
        nodes[root].dept_first_visit();
    }

    void fill_father_intersection(){
        nodes[root].propagate_father_intersection(nullptr);
    }

};



Node* common_parent(Node* n1, Node* n2){
    Node* parent;

    if(n1->discovery_time < n2->discovery_time){
        parent = n1;
    }else{
        parent = n2;
    }

    // go up the father until the condition is met
    while(!(parent->is_ancestor_of(n1) && parent->is_ancestor_of(n2))){
        parent = parent->father_intersection;
    }
    return parent;
}


Node* next_step(Node* from, Node* to){

    Node* next_step = from;

    for(auto adj: from->adjacent_nodes){
        if(adj->is_ancestor_of(next_step)){
            continue;
        }
        if(adj->is_ancestor_of(to)){
            next_step = adj;
        }
    }

    return next_step;
}

bool are_adjacent(Node* n1, Node* n2){
    assert(n1!=n2);
    for(auto adj: n1->adjacent_nodes){
        if(adj == n2){
            return true;
        }
    }
    return false;
}

// calculate the distance between to node, (each one madke part of a different group, where node 2 is an ancestor of node 1
int distance_linear(Node* node_from, Node* node_to){
    if(node_from == node_to){
        return 0;
    }
    if(node_to->distance_from_root == node_from->distance_from_root){
        return 1;
    }

    Node* oldest;
    Node* youngest;

    if(node_from->distance_from_root < node_to->distance_from_root){
        oldest = node_from;
        youngest = node_to;
    }else{
        oldest = node_to;
        youngest = node_from;
    }

    Node* next = next_step(oldest,youngest);

    return 1 + next->distance_from_root - youngest->distance_from_root;
}

// calculate the distance between to nodes, that are not one the ancestor of the other, but have a common ancestor
int distance_on_tree(Node* n1, Node* n2, Node* common_ancestor){

    Node* next_step_to_n1 = next_step(common_ancestor,n1);
    Node* next_step_to_n2 = next_step(common_ancestor,n2);

    int d = n1->distance_from_root + n2 ->distance_from_root - next_step_to_n2->distance_from_root - next_step_to_n1->distance_from_root;

    if(are_adjacent(next_step_to_n2,next_step_to_n1)){
        d++;
    }else{
        d+=2;
    }
    return d;
}


int calculate_distance_2(Graph& graph,Node* n1, Node* n2){

    if(n1 == n2){
        return 0;
    }

    queue<int> distances = queue<int>();
    queue<Node*> to_visit = queue<Node*>();

    vector<bool> is_pushed = vector<bool>();
    is_pushed.reserve(graph.nodes.size());

    for(int i=0; i<graph.nodes.size(); i++){
        is_pushed.push_back(false);
    }

    to_visit.push(n1);
    distances.push(0);
    is_pushed[n1->value] = true;

    while (!to_visit.empty()){

        Node* node = to_visit.front();
        to_visit.pop();
        int distance = distances.front();
        distances.pop();

        for(auto adj: node->adjacent_nodes){
            if(is_pushed[adj->value]){
                continue;
            }
            if(adj == n2){
                return distance+1;
            }
            to_visit.push(adj);
            distances.push(distance+1);
        }

    }
    assert(false);
    return -1;
}

int main(){

    int n_nodes, n_links, n_questions;


    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes >> n_links >> n_questions;

    Graph graph = Graph(n_nodes);


    for(int i=0; i<n_links; i++){
        int n1,n2;
        input >> n1 >> n2;
        graph.insert_link(n1,n2);
    }

    // calculate the color (aka the fully connected part of the graph
    graph.propagate_distance_from_root();
    graph.insert_times();
    graph.fill_father_intersection();

    //cout << graph.nodes << endl;


    for(int i=0; i<n_questions; i++){
        int from,to;
        input >> from >> to;

        Node* node_from = &graph.nodes[from];
        Node* node_to = &graph.nodes[to];

        Node* parent = common_parent(node_from,node_to);

        int distance;// = node_from->distance_from_root + node_to->distance_from_root - 2 * parent->distance_from_root;

        if(parent == node_from || parent == node_to){
            distance = distance_linear(node_from,node_to);
        }else{
            distance = distance_on_tree(node_to,node_from,parent);
        }

        //int actual_distance = calculate_distance_2(graph,node_to,node_from);
        //cout << distance << " " << actual_distance << endl;

        output << distance << endl;
    }

    output.close();
    input.close();

}


ostream & operator<<(ostream & os, Node& node){
    int father_intersection = -1;
    if(node.father_intersection!= nullptr){
        father_intersection = node.father_intersection->value;
    }
    os << "{id:  " << node.value << ", distance: " << node.distance_from_root << ", father intersection: " << father_intersection << ", dt: " << node.discovery_time << ", ft: " << node.finish_time << "}";
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