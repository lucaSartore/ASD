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
int current_color;

class Node{
public:
    int value;
    int discovery_time;
    vector<Node*> adjacent_nodes;
    int color;
    Node* has_same_color_of;
    Node(int n){
        value = n;
        discovery_time = -1;
        adjacent_nodes = vector<Node*>();
        color = -1;
        has_same_color_of = nullptr;
    }

    void insert_adjacent_node(Node* adjacent){
        adjacent_nodes.push_back(adjacent);
    }

    // return a pointer to the oldest node that he found
    Node* fill_has_same_color_of(Node* coming_from){

        discovery_time = current_time;
        current_time++;

        has_same_color_of = nullptr;

        // compute same color first, as this make shore that n
        for(auto adjacent_node: adjacent_nodes) {

            if(adjacent_node == coming_from){
                continue;
            }

            Node* new_has_same_color_of = nullptr;

            if(adjacent_node->discovery_time == -1){
                //need to discover the new node
                new_has_same_color_of = adjacent_node->fill_has_same_color_of(this);
                if(new_has_same_color_of == this){
                    new_has_same_color_of = nullptr;
                }
            }else if(adjacent_node->discovery_time < discovery_time){
                // if the node i am looking to has already been discovered, and has also been discovered before me
                // then i have created a cycle, and he is the original generator
                new_has_same_color_of = adjacent_node;
            }

            // now keep the same color that has been discovered earlyer
            if(new_has_same_color_of == nullptr){
                continue;
            }
            if(has_same_color_of == nullptr){
                has_same_color_of = new_has_same_color_of;
                continue;
            }
            // when i have two valid color, i take the earlyest one.
            if(new_has_same_color_of->discovery_time < has_same_color_of->discovery_time){
                has_same_color_of = new_has_same_color_of;
            }
        }

        return has_same_color_of;
    }

    int get_color(){
        if(color != -1){
            return color;
        }
        if(has_same_color_of != nullptr){
            color = has_same_color_of->get_color();
            return color;
        }
        color = current_color;
        current_color++;
        return color;
    }
};


class Graph{
public:
    vector<Node> nodes;

    Graph(int n_nodes){
        nodes = vector<Node>();
        nodes.reserve(n_nodes);
        for(int i=0; i<n_nodes; i++){
            nodes.emplace_back(i);
        }
    }

    void insert_link(int n1, int n2){
        nodes[n1].insert_adjacent_node(&nodes[n2]);
        nodes[n2].insert_adjacent_node(&nodes[n1]);
    }



    void fill_has_same_color_of(){
        current_time = 0;
        nodes[0].fill_has_same_color_of(nullptr);
        /*for(auto& node: nodes) {
            if (node.has_same_color_of == nullptr) {
                node.fill_has_same_color_of(nullptr);
            }
        }*/
    }

    // color the graph, and return te number of colors he found
    int color_graph(){
        current_color = 0;

        for(auto &node: nodes){
            if(node.color == -1){
                node.get_color();
            }
        }

        return current_color;
    }


};

class GroupNode;

struct Link{
public:
    GroupNode* to;
    Node* linking_node_from;
    Node* linking_node_to;

    Link(){
        to = nullptr;
        linking_node_from = nullptr;
        linking_node_to = nullptr;
    }

    Link(GroupNode* _to, Node* _linking_node_from, Node* _linking_node_to){
        to = _to;
        linking_node_from = _linking_node_from;
        linking_node_to = _linking_node_to;
    }

    inline bool is_null(){
        return to == nullptr;
    }
};

class GroupNode {
public:
    vector<Link> adjacent_nodes;
    Link father_intersection;
    int color;
    int discovery_time;
    int finish_time;
    bool is_root;
    int distance_from_root;

    GroupNode(int _color) {
        color = _color;
        discovery_time = -1;
        finish_time = -1;
        adjacent_nodes = vector<Link>();
        father_intersection = Link(nullptr, nullptr, nullptr);
        is_root = false;
        distance_from_root = -1;
    }

    void insert_adjacent_group(GroupNode *adjacent, Node *linking_node_from, Node *linking_node_to) {
        adjacent_nodes.push_back(Link(adjacent, linking_node_from, linking_node_to));
    }

    // this node is an itersection if it hase more that 3 adjacent nodes
    bool is_intersection() {
        return adjacent_nodes.size() >= 3;
    }

    void dept_first_visit() {

        discovery_time = current_time;
        current_time++;

        for (auto &adjacent: adjacent_nodes) {
            if (adjacent.to->discovery_time != -1) {
                continue;
            }
            adjacent.to->dept_first_visit();
        }
        finish_time = current_time;
        current_time++;
    }

    void propagate_father_intersection(Link new_father_intersection) {
        father_intersection = new_father_intersection;

        if (is_intersection()) {
            new_father_intersection = Link(this, nullptr, nullptr);
        }

        for (auto &adjacent: adjacent_nodes) {
            // skip the father
            if (adjacent.to->discovery_time < discovery_time) {
                continue;
            }
            new_father_intersection.linking_node_from = adjacent.linking_node_to;
            if (is_intersection()) {
                new_father_intersection.linking_node_to = adjacent.linking_node_from;
            }
            adjacent.to->propagate_father_intersection(new_father_intersection);
        }
    }

    void propagate_distance_from_root(Node *coming_from) {
        for (auto &adjacent: adjacent_nodes) {
            if (adjacent.to->distance_from_root != -1) {
                continue;
            }
            int to_add = 1;
            if (adjacent.linking_node_from != coming_from && coming_from != nullptr) {
                to_add++;
            }
            adjacent.to->distance_from_root = distance_from_root + to_add;
            adjacent.to->propagate_distance_from_root(adjacent.linking_node_to);
        }
    }

    // node: a node is ancestor of himself
    bool is_ancestor_off(GroupNode* tested_child){
        return discovery_time <= tested_child->discovery_time && finish_time >= tested_child->discovery_time;
    }
};

class GroupGraph{
public:
    vector<GroupNode> groups;
    int root;
    GroupGraph(int n_groups, int _root = 0){
        groups = vector<GroupNode>();
        groups.reserve(n_groups);
        for(int i=0; i<n_groups; i++){
            groups.emplace_back(i);
        }
        root = _root;
        groups[root].is_root = true;
    }

    void insert_link(Node* from, Node* to){
        int color_from = from->color;
        int color_to = to->color;
        groups[color_from].insert_adjacent_group(&groups[color_to],from,to);
        //groups[color_to].insert_adjacent_group(&groups[color_from],to,from);
    }

    void insert_times(){
        current_time = 0;
        groups[root].dept_first_visit();
    }


    void propagate_distance_from_root(){
        groups[root].distance_from_root = 0;
        groups[root].propagate_distance_from_root(nullptr);
    }

    void fill_father_intersection(){
        groups[root].propagate_father_intersection(Link(nullptr, nullptr, nullptr));
    }
};


GroupNode* common_parent(GroupNode* n1, GroupNode* n2){
    GroupNode* parent;

    if(n1->discovery_time < n2->discovery_time){
        parent = n1;
    }else{
        parent = n2;
    }

    // go up the father until the condition is met
    while(!(parent->is_ancestor_off(n1) && parent->is_ancestor_off(n2))){
        parent = parent->father_intersection.to;
    }
    return parent;
}

int extra_steps_linear(GroupGraph& graph, Node* father, Node* child){
    GroupNode* father_group = &graph.groups[father->color];
    GroupNode* child_group = &graph.groups[child->color];

    int to_return = 0;

    for(auto& father_group_adjacent: father_group->adjacent_nodes){
        if(father_group_adjacent.to->is_ancestor_off(father_group)){
            continue;
        }
        if(father_group_adjacent.to->is_ancestor_off(child_group)){
            if(father != father_group_adjacent.linking_node_from){
                to_return++;
            }
            break;
        }
    }

    for(auto& child_group_adjacent: child_group->adjacent_nodes){
        if(child_group_adjacent.to->is_ancestor_off(child_group)){
            if(child_group_adjacent.linking_node_from != child){
                to_return++;
            }
        }
    }

    return to_return;
}

int extra_steps_tree(GroupGraph& graph, Node* n1, Node* n2, GroupNode* common){

    GroupNode* n1_group = &graph.groups[n1->color];
    GroupNode* n2_group = &graph.groups[n2->color];

    int to_return = 0;

    for(auto& child_group_n1: n1_group->adjacent_nodes){
        if(child_group_n1.to->is_ancestor_off(n1_group)){
            if(child_group_n1.linking_node_from != n1){
                to_return++;
            }
        }
    }

    for(auto& child_group_n2: n2_group->adjacent_nodes){
        if(child_group_n2.to->is_ancestor_off(n2_group)){
            if(child_group_n2.linking_node_from != n2){
                to_return++;
            }
        }
    }

    Node* n_to_1;
    Node* n_to_2;


    for(auto& common_adjacent: common->adjacent_nodes){
        if(common_adjacent.to->is_ancestor_off(common)){
            continue;
        }
        if(common_adjacent.to->is_ancestor_off(n1_group)){
            n_to_1 = common_adjacent.linking_node_from;
        }
        if(common_adjacent.to->is_ancestor_off(n2_group)){
            n_to_2 = common_adjacent.linking_node_from;
        }
    }

    if(n_to_2 == n_to_1){
        to_return++;
    }

    return to_return;
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
    graph.fill_has_same_color_of();
    int n_color = graph.color_graph();

    GroupGraph group_graph = GroupGraph(n_color);

    //cout << graph.nodes << endl;

    for(auto& node: graph.nodes) {
        for (auto adjacent: node.adjacent_nodes) {
            if (adjacent->color != node.color) {
                group_graph.insert_link(&node, adjacent);
            }
        }
    }

    group_graph.insert_times();
    group_graph.fill_father_intersection();
    group_graph.propagate_distance_from_root();

    cout << group_graph.groups << endl;

    for(int i=0; i<n_questions; i++){
        int from,to;
        input >> from >> to;

        Node* node_from = &graph.nodes[from];
        Node* node_to = &graph.nodes[to];

        GroupNode* group_from = &group_graph.groups[node_from->color];
        GroupNode* group_to = &group_graph.groups[node_to->color];

        if(group_from->color == group_to->color){
            if(node_from == node_to){
                output << 0;
            } else{
                output << 1;
            }
            continue;
        }

        GroupNode* common_father = common_parent(group_from,group_to);

        int approximate_distance = group_from->distance_from_root + group_to->distance_from_root
                - 2*common_father->distance_from_root;
        //cout << "n1: " << group_from << endl;
        //cout << "n2: " << group_to << endl;
        //cout << "common: " << common_father << endl;

        int difference = 0;

        if(group_from->is_ancestor_off(group_to)){
            difference = extra_steps_linear(group_graph,node_from,node_to);
        }else if(group_to->is_ancestor_off(group_from)){
            difference = extra_steps_linear(group_graph,node_to,node_from);
        }else{
            difference = extra_steps_tree(group_graph,node_from,node_to,common_father);
        }
        int distance = approximate_distance + difference;

        cout << "From " << from << " To " << to << ": " << distance << " approximate: " << approximate_distance << endl;

        output << distance << endl;
    }

    output.close();
    input.close();

}


ostream & operator<<(ostream & os, Node& node){
    int same_color;
    if(node.has_same_color_of == nullptr){
        same_color = -1;
    }else{
        same_color = node.has_same_color_of->value;
    }
    os << "{id:  " << node.value <<", color: " << node.color  <<  ", has_same_color_of: " <<  same_color << "}";
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

ostream & operator<<(ostream & os, GroupNode* node){
    return os << *node;
}
ostream & operator<<(ostream & os, GroupNode& node){
    int father_intersection = -1;
    if(node.father_intersection.to != nullptr){
        father_intersection = node.father_intersection.to->color;
    }
    os << "{color: "<< node.color << " dt: " << node.discovery_time << " ft: " << node.finish_time << " father_intersection: " << father_intersection << ", distance: "<< node.distance_from_root <<"}";
    return os;
}