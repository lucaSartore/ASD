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


tuple<Node*,int> farthest_node_from(Graph& graph, Node* node){

    vector<bool> visited = vector<bool>();
    visited.reserve(graph.nodes.size());
    for(int i=0; i<graph.nodes.size(); i++){
        visited.push_back(false);
    }

    // breath first search
    queue<tuple<Node*,int>> order_visit = queue<tuple<Node*,int> >();
    visited[node->value] = true;
    order_visit.emplace(node,0);

    tuple<Node*,int> farthest_node = tuple<Node*,int>(node,0);

    while(!order_visit.empty()){

        auto t =  order_visit.front();

        Node* next_node = get<0>(t);
        int distance = get<1>(t);

        order_visit.pop();
        farthest_node = t;

        for(auto adjacent: next_node->adjacent_nodes){
            if(visited[adjacent->value]){
                continue;
            }
            visited[adjacent->value] = true;

            order_visit.emplace(adjacent,distance+1);
        }
    }
    return farthest_node;
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

    // this node is in between the longest path
    auto n1 = farthest_node_from(graph, &graph.nodes[0]);

    // this node is one of the two extremis of the longest path
    auto n2 = farthest_node_from(graph, get<0>(n1));

    // this node is the other extremis on the longest pat
    auto n3 = farthest_node_from(graph, get<0>(n2));

    int distance =  get<1>(n3);

    cout << "distance: " << distance;

    output << distance;

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
