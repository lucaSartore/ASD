#include <iostream>
#include <fstream>
#include <vector>
#include <execution>
using namespace std;

typedef enum {FAB_LAB, STUDENTS, IMPOSTOR, NONE} NodeType;

class Node;

class Link{
public:
    Node* to;
    int cost_min;
    int cost_max;
    int current_cost;

    Link(Node* to, int cost_min, int cost_max){
        this->to = to;
        this->cost_min = cost_min;
        this->cost_max = cost_max;
        this->current_cost = cost_min;
    }
};

class LinkReversed{
public:
    Node* to;
    Link* original_link;

    LinkReversed(Node* to, Link* original_link){
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
    Node(int n){
        value = n;
        node_type = NONE;
        adjacent_nodes = vector<Link>();
        adjacent_nodes_reversed = vector<LinkReversed>();
    }

    Link* insert_adjacent_node(Link adjacent){
        adjacent_nodes.push_back(adjacent);
        return &adjacent_nodes.back();
    }

    void insert_adjacent_node_reversed(LinkReversed adjacent){
        adjacent_nodes_reversed.push_back(adjacent);
    }

};


class Graph{
public:
    vector<Node> nodes;

    Graph(int n_nodes, int pos_impostor, int pos_students, int pos_fab_lab){
        nodes = vector<Node>();
        nodes.reserve(n_nodes);
        for(int i=0; i<n_nodes; i++){
            nodes.emplace_back(i);
        }
        nodes[pos_impostor].node_type = IMPOSTOR;
        nodes[pos_students].node_type = STUDENTS;
        nodes[pos_fab_lab].node_type = FAB_LAB;
    }

    void insert_link(int from, int to, int cost_min, int cost_max){
        Link* link = nodes[from].insert_adjacent_node(Link(&nodes[to],cost_min,cost_max));
        nodes[to].insert_adjacent_node_reversed(LinkReversed(&nodes[from],link));
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
        graph.insert_link(from,to,cost,cost);
    }

    for(int i=0; i<n_special_links; i++){
        int from,to,cost_min,cost_max;
        input >> from >> to >> cost_min >> cost_max;
        graph.insert_link(from,to,cost_min,cost_max);
    }

    output.close();
    input.close();
}