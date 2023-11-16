#include <istream>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <tuple>
using namespace std;

class Node;

class Node{
public:
    int value;
    vector<Node*> adjacent_nodes;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Node*>();
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
            nodes.push_back(Node(i));
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


int distance(Graph& graph, Node* node_1, Node* node_2){

    vector<bool> visited = vector<bool>();
    visited.reserve(graph.nodes.size());
    for(int i=0; i<graph.nodes.size(); i++){
        visited.push_back(false);
    }

    // breath first search
    queue<tuple<Node*,int>> order_visit = queue<tuple<Node*,int>>();
    visited[node_1->value] = true;
    order_visit.emplace(node_1,0);

    while(!order_visit.empty()){
        auto t =  order_visit.front();

        Node* next_node = get<0>(t);
        int distance = get<1>(t);
        order_visit.pop();

        for(auto adjacent: next_node->adjacent_nodes){
            if(visited[adjacent->value]){
                continue;
            }
            visited[adjacent->value] = true;
            if(adjacent == node_2){
                return distance+1;
            }
            order_visit.emplace(adjacent,distance+1);
        }
    }
    return -1;
}

int num_path_to_reach(Graph& graph, Node* from, Node* to, int steps, vector<bool>& visited){

    if(steps == 0){
        return 0;
    }

    visited[from->value] = true;

    int count = 0;

    for(auto adjacent_node: from->adjacent_nodes){
        if(adjacent_node == to){
            return 1;
        }
        if(visited[adjacent_node->value]){
            continue;
        }
        count += num_path_to_reach(graph,adjacent_node, to,steps-1,visited);
    }

    return count;
}

int main(){
    int n_nodes;
    int n_edges;
    int node_1;
    int node_2;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_edges;
    input >> node_1;
    input >> node_2;

    Graph graph(n_nodes);

    for(int i=0; i<n_edges; i++){
        int n1,n2;
        input >> n1;
        input >> n2;
        graph.insert_double_edge(n1, n2);
    }

    vector<bool> visited = vector<bool>();
    visited.reserve(n_nodes);
    for(int i=0; i<n_nodes; i++){
        visited.push_back(false);
    }

    int distance_n1_n2 = distance(graph,&graph.nodes[node_1],&graph.nodes[node_2]);
    int num_paths = num_path_to_reach(graph,&graph.nodes[node_1],&graph.nodes[node_2],distance_n1_n2,visited);


    cout << "Distance: " << distance_n1_n2 << endl;

    cout << "Num path: " << num_paths << endl;



    output.close();
    input.close();
}