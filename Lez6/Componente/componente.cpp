#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<int> order_visit;
int current_time;


class Node{
public:
    int value;
    int color;
    int time_discovery;
    int time_end;
    vector<Node*> adjacent_nodes;
    vector<Node*> adjacent_nodes_reversed;
    Node(int n){
        value = n;
        color = -1;
        time_discovery = -1;
        time_end = -1;
        adjacent_nodes = vector<Node*>();
    }

    void insert_adjacent_node(Node* adjacent){
        adjacent_nodes.push_back(adjacent);
    }

    void insert_adjacent_node_reversed(Node* adjacent){
        adjacent_nodes_reversed.push_back(adjacent);
    }
    void propagate_time(){
        order_visit.push_back(value);
        time_discovery = current_time;
        current_time++;
        for(auto adjacent: adjacent_nodes){
            if(adjacent->time_discovery == -1){
                adjacent->propagate_time();
            }
        }
        time_end = current_time;
        current_time++;
    }

    void propagate_color(vector<int> &color_count, int super_time_discovery, int super_time_end){
        for(auto node: adjacent_nodes_reversed){
            if(node->color != -1) continue;

            if(super_time_discovery < node->time_discovery &&
               super_time_end > node->time_end){
                node->color = color;
                color_count[color]++;
                node->propagate_color(color_count,super_time_discovery,super_time_end);
            }
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

    order_visit = vector<int>();
    order_visit.reserve(n_nodes);
    current_time = 0;

    for(auto &node: graph.nodes){
        if(node.time_discovery == -1){
            node.propagate_time();
        }
    }

    vector<int> color_count = vector<int>();

    int color = 0;
    for(int node_id: order_visit){

        auto node = &graph.nodes[node_id];

        if(node->color != -1) continue;

        color_count.push_back(1);

        node->color = color;
        color++;

        node->propagate_color(color_count,node->time_discovery,node->time_end);
    }

    int max_color = 0;
    for(int v: color_count){
        max_color = max(v,max_color);
    }

    output << max_color;

    output.close();
    input.close();
}