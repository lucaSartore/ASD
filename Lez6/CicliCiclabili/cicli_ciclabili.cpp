#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <queue>
#include <algorithm>
using namespace std;


class Node;

ostream & operator<<(ostream & os, Node* node);
ostream & operator<<(ostream & os, Node& node);
template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);


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

    void reset_discovery_time(){
        current_time = 0;
        for(auto& node: nodes){
            node.discovery_time = -1;
        }
    }

    void fill_has_same_color_of(){
        reset_discovery_time();
        for(auto& node: nodes) {
            if (node.has_same_color_of == nullptr) {
                node.fill_has_same_color_of(nullptr);
            }
        }
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
    GroupNode* to;
    Node* linking_node;

    Link(GroupNode* _to, Node* _linking_node){
        to = _to;
        linking_node = _linking_node;
    }
};

class GroupNode{
public:
    vector<Link> adjacent_nodes;
    int color;
    bool visited;

    GroupNode(int _color,int n_nodes){
        color = _color;
        visited = false;
    }


    void insert_adjacent_group(GroupNode* adjacent, Node* border_node){
        adjacent_nodes.push_back(Link(adjacent,border_node));
    }

    int distance_from(Node* starting_node, Node* destination_node){

        if(starting_node == destination_node){
            return 0;
        }
        if(starting_node->color == destination_node->color){
            return 1;
        }

        for(auto& adjacent: adjacent_nodes){
            int new_distance = adjacent.to->distance_from(adjacent.linking_node,destination_node);
            if(new_distance != -1){
                if(starting_node == adjacent.linking_node){
                    return new_distance+1;
                }else{
                    return new_distance+2;
                }
            }
        }


    }
};

class GroupGraph{
public:
    vector<GroupNode> groups;

    GroupGraph(int n_groups){
        groups = vector<GroupNode>();
        groups.reserve(n_groups);
        for(int i=0; i<n_groups; i++){
            groups.emplace_back(i,n_groups);
        }
    }

    void insert_link(Node* from, Node* to){
        int color_from = from->color;
        int color_to = to->color;
        int link_node = from->value;
        groups[color_from].insert_adjacent_group(&groups[color_to],link_node);
    }

    vector<int> best_path(int from, int to){

        if(from == to){
            auto v = vector<int>();
            v.push_back(from);
            return v;
        }

        reset_visited();

        queue<GroupNode*> to_visit = queue<GroupNode*>();

        vector<GroupNode*> hwo_pushed_me = vector<GroupNode*>();
        hwo_pushed_me.reserve(groups.size());
        for(int i=0; i<groups.size(); i++){
            hwo_pushed_me.push_back(nullptr);
        }

        to_visit.push(&groups[from]);
        groups[from].visited = true;

        bool has_been_found = false;

        while (!has_been_found){


            GroupNode* current_node = to_visit.front();
            to_visit.pop();

            for(auto adjacent: current_node->adjacent_nodes){
                if(adjacent->visited){
                    continue;
                }

                to_visit.push(adjacent);
                adjacent->visited = true;
                hwo_pushed_me[adjacent->color] = current_node;

                if(adjacent->color == to){
                    has_been_found = true;
                    break;
                }
            }
        }

        vector<int> to_return = vector<int>();

        int current = to;

        while (true){

            to_return.push_back(current);

            if(current == from){
                break;
            }

            current = hwo_pushed_me[current]->color;
        }

        reverse(to_return.begin(),to_return.end());

        return to_return;
    }

    void reset_visited(){
        for(auto& group: groups){
            group.visited = false;
        }
    }

};

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

    return 0;

    /// insert the hasmap only to the nodes that have more then one node to save memory
    vector<int> color_count = vector<int>();
    color_count.reserve(n_color);
    for(int i=0; i<n_color; i++){
        color_count.push_back(0);
    }

    for(auto& node: graph.nodes){
        color_count[node.color]++;
    }

    for(int i=0; i<n_color; i++){
        if(color_count[i]>1){
            group_graph.groups[i].has_more_than_one_node = true;
        }
    }

    for(auto& node: graph.nodes) {
        for (auto adjacent: node.adjacent_nodes) {
            if (adjacent->color != node.color) {
                group_graph.insert_link(&node, adjacent);
            }
        }
    }

    for(auto& group: group_graph.groups){
        group.distance_from_node[group.color] = 0;
        group.propagate_distance(group.color,group.color);
    }

    for(int i=0; i<n_questions; i++){

        int from,to;
        input >> from >> to;

        int color_from,color_to;
        color_from = graph.nodes[from].color;
        color_to = graph.nodes[to].color;

        // basic cases
        if(from == to){
            output << 0 << endl;
            continue;
        }
        if(color_from == color_to){
            output << 1 << endl;
            continue;
        }

        int base_distance = group_graph.groups[color_from].distance_from_node[color_to];

        if(group_graph.groups[color_from].next_hop[color_to] != from &&
            group_graph.groups[color_from].has_more_than_one_node
        ){
            base_distance++;
        }

        if(group_graph.groups[color_to].next_hop[color_from] != to &&
            group_graph.groups[color_to].has_more_than_one_node
        ){
            base_distance++;
        }

        output << base_distance << endl;
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
