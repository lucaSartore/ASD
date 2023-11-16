#include <istream>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <tuple>
#include <algorithm>
// Nota: il grafo è connesso

using namespace std;

# define UNKNOWN -1

class Node;

ostream & operator<<(ostream & os, Node* node);
ostream & operator<<(ostream & os, list<Node*>& path);
template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);


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




int max_distance_to_all(Graph& graph, Node* node){

    vector<int> to_return = vector<int>();
    vector<bool> visited = vector<bool>();
    visited.reserve(graph.nodes.size());
    to_return.reserve(graph.nodes.size());
    for(int i=0; i<graph.nodes.size(); i++){
        visited.push_back(false);
        to_return.push_back(0);
    }

    // breath first search
    queue<tuple<Node*,int>> order_visit = queue<tuple<Node*,int> >();
    visited[node->value] = true;
    to_return[node->value] = 0;
    order_visit.emplace(node,0);

    int max_distance = 0;

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
            max_distance = distance+1;
            order_visit.emplace(adjacent,distance+1);
        }
    }
    return max_distance;
}

int main(){
    int n_nodes;
    int n_edges;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_edges;


    Graph graph(n_nodes);

    for(int i=0; i<n_edges; i++){
        int n1,n2;
        input >> n1;
        input >> n2;
        graph.insert_double_edge(n1, n2);
    };

    int max_distance = 0;

    for(int i=0; i<n_nodes; i++){
        int new_distance = max_distance_to_all(graph,&graph.nodes[i]);
        max_distance = max(new_distance,max_distance);
    }

    cout << "distance: " << max_distance;

    output << max_distance;

    output.close();
    input.close();
}



ostream & operator<<(ostream & os, list<Node*>& path){
    os << "[";
    for(auto node: path){
        os << " -> " << node->value;
    }
    os << "]";
    return os;
}

ostream & operator<<(ostream & os, Node* node){
    os << "Node " << node->value;
    return os;
}
ostream & operator<<(ostream & os, Node node){
    os << "Node " << node.value;
    return os;
}


template<typename T>
ostream & operator<<(ostream & os, vector<T>& v){
    os << "[";
    int c = 0;
    for(auto node: v){
        os << "\t" << c <<": " << node << "\n";
        c++;
    }
    os << "]\n";
    return os;
}
