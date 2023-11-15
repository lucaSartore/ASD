#include <istream>
#include <vector>
#include <fstream>
#include <iostream>

// Nota: il grafo è connesso

using namespace std;

# define UNKNOWN -1

class Node{
    public:
    int value;
    vector<Node*> adjacent_nodes;
    vector<int> distance;

    Node(int _value, int total_nodes){
        value = _value;
        adjacent_nodes = vector<Node*>();
        distance = vector<int>();
        distance.reserve(total_nodes);
        for(int i=0; i<total_nodes; i++){
            distance.push_back(UNKNOWN);
        }
    }

    void insert_adjacent_node(Node* node_to_insert) {
        adjacent_nodes.push_back(node_to_insert);
    }
};



class Graph{
public:
    vector<Node> nodes;

    Graph(int number_of_nodes){
        nodes = vector<Node>();
        nodes.reserve(number_of_nodes);
        for(int i=0; i<number_of_nodes; i++){
            nodes.push_back(Node(i,number_of_nodes));
        }
    }

    void insert_edge(int from, int to){
        nodes[from].insert_adjacent_node(&nodes[to]);
    }
    
    void insert_double_edge(int n1, int n2){
        insert_edge(n1,n2);
        insert_edge(n2,n1);
    }
};

int main(){
    int n_nodes;
    int n_edges;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_edges;


    Graph graph(n_nodes);
    vector<bool> reachable = vector<bool>();
    reachable.reserve(n_nodes);

    for(int i=0; i<n_edges; i++){
        int from,to;
        input >> from;
        input >> to;
        graph.insert_edge(from, to);
        reachable.push_back(false);
    };

}