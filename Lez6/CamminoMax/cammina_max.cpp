#include <iostream>
#include <fstream>
#include <vector>
#include <execution>
using namespace std;

class Node{
public:
    int value;
    bool is_pushed;
    int cammino_max;
    vector<Node*> adjacent_nodes;
    vector<Node*> adjacent_nodes_reversed;
    Node(int n){
        value = n;
        is_pushed = false;
        cammino_max = -1;
        adjacent_nodes = vector<Node*>();
        adjacent_nodes_reversed = vector<Node*>();
    }

    int find_possible_start(){
        if(adjacent_nodes_reversed.empty()){
            return value;
        }
        for(auto adjacent_node: adjacent_nodes_reversed){
            if(!adjacent_node->is_pushed){
                return adjacent_node->find_possible_start();
            }
        }
        throw runtime_error("this should be unreachable");
    }

    void insert_adjacent_node(Node* adjacent){
        adjacent_nodes.push_back(adjacent);
    }

    void insert_adjacent_node_reversed(Node* adjacent){
        adjacent_nodes_reversed.push_back(adjacent);
    }

    void fill_topological_order(vector<int>& topological_order){
        int cammino_max_temp = -1;
        for(auto adjacent_node: adjacent_nodes_reversed){
            if(!adjacent_node->is_pushed){
                return;
            }
            cammino_max_temp = max(cammino_max_temp, adjacent_node->cammino_max);
        }
        topological_order.push_back(value);
        cammino_max = cammino_max_temp + 1;
        is_pushed = true;
        for(auto adjacent_node: adjacent_nodes){
            if(adjacent_node->is_pushed){
                continue;
            }
            adjacent_node->fill_topological_order(topological_order);
        }
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

    void insert_link(int from, int to){
        nodes[from].insert_adjacent_node(&nodes[to]);
        nodes[to].insert_adjacent_node_reversed(&nodes[from]);
    }
};

int main(){

    int n_nodes, n_links;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes >> n_links;

    Graph graph = Graph(n_nodes);

    for(int i=0; i<n_links; i++){
        int from,to;
        input >> from >> to;
        graph.insert_link(from,to);
    }

    vector<int> topological_order = vector<int>();
    topological_order.reserve(n_nodes);


    for(auto& node: graph.nodes){

         if(node.is_pushed){
            continue;
        }

        int start_id = node.find_possible_start();
        graph.nodes[start_id].cammino_max = 0;
        graph.nodes[start_id].fill_topological_order(topological_order);

    }

    int cammino_max = 0;

    for(auto& node: graph.nodes){
        cammino_max = max(cammino_max,node.cammino_max);
    }


    output << cammino_max << " ";


    output.close();
    input.close();
}