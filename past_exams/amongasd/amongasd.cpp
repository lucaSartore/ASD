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
# define UNREACHABLE -2

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
    int min_distance_from_fab_lab;
    int max_distance_from_fab_lab;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Link>();
        min_distance_from_fab_lab = UNKNOWN;
        max_distance_from_fab_lab = UNKNOWN;
    }

    void insert_adjacent_node(Node* node_to_insert, int min_cost, int max_cost) {
        adjacent_nodes.emplace_back(node_to_insert,min_cost, max_cost);
    }

    int get_min_distance_from_fab_lab(){
        if(min_distance_from_fab_lab != UNKNOWN){
            return min_distance_from_fab_lab;
        }
        min_distance_from_fab_lab = UNREACHABLE;

        for(auto adjacent: adjacent_nodes){
            int new_min_distance = adjacent.node->get_min_distance_from_fab_lab();
            if(new_min_distance == UNREACHABLE){
                continue;
            }
            new_min_distance+=adjacent.min_cost;
            min_distance_from_fab_lab = min(min_distance_from_fab_lab,new_min_distance);
        }
        return min_distance_from_fab_lab;
    }

    int get_max_distance_from_fab_lab(){

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