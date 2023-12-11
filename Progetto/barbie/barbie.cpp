#include <istream>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <tuple>
#include <unordered_set>
#include <algorithm>
#include <set>
#include <unordered_map>
// Nota: il grafo è connesso

using namespace std;

class Node;

class Link{
public:
    int cost;
    Node* node;

    Link(Node* _node, int _cost ){
        node = _node;
        cost = _cost;
    }
};

class Node{
public:
    int value;
    vector<Link> adjacent_nodes;
    bool is_occupied;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Link>();
        is_occupied = false;
    }

    void insert_adjacent_node(Node* node_to_insert, int cost) {
        adjacent_nodes.emplace_back(node_to_insert,cost);
    }

};

class Graph{
public:
    vector<Node> nodes;

    explicit Graph(int number_of_nodes){
        nodes = vector<Node>();
        nodes.reserve(number_of_nodes);
        for(int i=0; i<number_of_nodes; i++){
            nodes.emplace_back(i);
        }
    }

    void insert_edge(int n1,int n2, int cost){
        nodes[n1].insert_adjacent_node(&nodes[n2],cost);
        nodes[n2].insert_adjacent_node(&nodes[n1],cost);
    }
};

int main(){
    int n_nodes;
    int n_edges;
    int n_occupied_nodes;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes >> n_edges;

    Graph graph(n_nodes);

    for(int i=0; i<n_nodes; i++){
        int n1,n2,cost;
        input >> n1 >> n2 >> cost;
        graph.insert_edge(n1,n2,cost);
    }

    input >> n_occupied_nodes;

    for(int i=0; i<n_occupied_nodes; i++){
        int occupied_node;
        input >> occupied_node;
        graph.nodes[occupied_node].is_occupied = true;
    }

    output.close();
    input.close();
}