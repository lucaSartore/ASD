#include <istream>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Node{
    public:
    int value;
    vector<Node*> adjacent_nodes;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Node*>();
    }

    void insert_adjacent_node(Node* node_to_insert){
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
            nodes.push_back(Node(i));
        }
    }

    void insert_edge(int from, int to){
        nodes[from].insert_adjacent_node(&nodes[to]);
    }
};





int count_reachable(Graph& graph, int start_node, vector<bool>& visited, int counter){
    for(auto node: graph.nodes[start_node].adjacent_nodes){
        if(visited[node->value]){
            continue;
        }
        visited[node->value] = true;
        counter++;
        counter = count_reachable(graph,node->value,visited,counter);
    }
    return counter;
}


int main(){
    int n_nodes;
    int n_edges;
    int start_node;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_edges;
    input >> start_node;

    Graph graph(n_nodes);
    vector<bool> reachable = vector<bool>();
    reachable.reserve(n_nodes);

    for(int i=0; i<n_edges; i++){
        int from,to;
        input >> from;
        input >> to;
        graph.insert_edge(from, to);
        reachable.push_back(false);
    }

    reachable[start_node] = true;

    int count = count_reachable(graph,start_node,reachable,1);

    cout << "count: " << count;

    output << count;

    input.close();
    output.close();

}