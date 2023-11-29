#include <iostream>
#include <fstream>
#include <vector>
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

    void fill_has_same_color_of(){

        discovery_time = current_time;
        current_time++;

        for(auto adjacent_node: adjacent_nodes) {

            // base exploration case, the node has not been discovered
            if (adjacent_node->discovery_time == -1) {
                adjacent_node->fill_has_same_color_of();
                continue;
            }

            // there is a cycle, and the node has been discovered

            if (has_same_color_of == nullptr) {
                has_same_color_of = adjacent_node;
            } else if (adjacent_node->discovery_time > has_same_color_of->discovery_time) {
                has_same_color_of = adjacent_node;
            }
        }
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
        nodes[0].fill_has_same_color_of();
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
    graph.color_graph();

    cout << graph.nodes;

}


ostream & operator<<(ostream & os, Node& node){
    os << "{id:  " << node.value <<", color: " << node.color  << "}";
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
