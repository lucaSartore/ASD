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

# define UNKNOWN -1

class Node;

class Link{
public:
    int min_cost;
    int max_cost;
    Node* node;

    Link(Node* _node, int _min_const, int _max_cost){
        node = _node;
        min_cost = _min_const;
        max_cost = _max_cost;
    }
};

class Node{
public:
    int value;
    vector<Link> adjacent_nodes;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Link>();
    }

    void insert_adjacent_node(Node* node_to_insert, int min_cost, int max_cost) {
        adjacent_nodes.emplace_back(node_to_insert,min_cost, max_cost);
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

    void insert_edge(int from, int to, int min_cost, int max_cost){
        nodes[from].insert_adjacent_node(&nodes[to],min_cost, max_cost);
    }
};

int main(){
    int n_nodes;
    int n_normal_link;
    int n_complex_link;
    int pos_impostor;
    int pos_students;
    int pos_fab_lab;


    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_normal_link;
    input >> n_complex_link;
    input >> pos_impostor;
    input >> pos_students;
    input >> pos_fab_lab;

    Graph graph(n_nodes);

    for(int i=0; i<n_normal_link; i++){
        int from,to,cost;
        input >> from;
        input >> to;
        input >> cost;
        graph.insert_edge(from,to,cost,cost);
    }

    for(int i=0; i<n_complex_link; i++){
        int from,to,min_cost,max_cost;
        input >> from;
        input >> to;
        input >> min_cost;
        input >> max_cost;
        graph.insert_edge(from,to,min_cost,max_cost);
    }

    output.close();
    input.close();
}