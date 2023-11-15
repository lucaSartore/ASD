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


void propagate_distance(Graph& graph, int node_id, vector<bool>& visited, vector<int>& distance_v, vector<list<Node*>> & path_v){

    // breath first search
    queue<tuple<Node*,int>> order_visit = queue<tuple<Node*,int>>();
    order_visit.push(tuple<Node*,int>(&graph.nodes[node_id],0));

    visited[node_id] = true;
    distance_v[node_id] = 0;
    path_v[node_id].push_back(&graph.nodes[node_id]);

    while(!order_visit.empty()){

        auto t = order_visit.front();
        order_visit.pop();

        Node* node = get<0>(t);
        int distance = get<1>(t);

        for(auto adjacent: node->adjacent_nodes){
            if(visited[adjacent->value]){
                continue;
            }
            visited[adjacent->value] = true;

            order_visit.push(tuple<Node*,int>(adjacent,distance+1));

            distance_v[adjacent->value] = distance+1;
            path_v[adjacent->value] = path_v[node->value];
            path_v[adjacent->value].push_back(adjacent);
        }
    }
}

int not_in_common_distance(list<Node*>& path1, list<Node*>& path2){

    int nodes_in_common = 0;

    auto path_1_iter = path1.begin();
    auto path_2_iter = path2.begin();

    while ((*path_1_iter)->value == (*path_2_iter)->value){
        nodes_in_common++;
        path_1_iter++;
        path_2_iter++;
        if(path_1_iter == path1.end() || path_2_iter == path2.end()){
            break;
        }
    }

    bool triangle_condition = false;

    auto n1_adjacent = &(*path_1_iter)->adjacent_nodes;
    auto n2 = *path_1_iter;

    if (std::find(n1_adjacent->begin(), n1_adjacent->end(), n2) != n1_adjacent->end()){
        triangle_condition = true;
    }

    int distance = path1.size() + path2.size() + 1 - 2*nodes_in_common;
    if(triangle_condition){
        distance--;
    }
    return distance;
}


int main(){
    int n_nodes;
    int n_edges;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_edges;


    Graph graph(n_nodes);

    vector<bool> visited = vector<bool>();
    vector<int> distance = vector<int>();
    // path[i] = list containing the shortest path between node 0 and node i
    vector<list<Node*>> path = vector<list<Node*>>();

    visited.reserve(n_nodes);
    distance.reserve(n_nodes);
    path.reserve(n_nodes);


    for(int i=0; i<n_nodes; i++){
        visited.push_back(false);
        distance.push_back(UNKNOWN);
        path.push_back(list<Node*>());
    }

    cout << visited;

    for(int i=0; i<n_edges; i++){
        int n1,n2;
        input >> n1;
        input >> n2;
        graph.insert_double_edge(n1, n2);
    };

    propagate_distance(graph,0,visited,distance,path);

    int max_distance = 0;

    for(int i=0; i<n_nodes; i++){
        for(int j=0; j<n_nodes; j++){
            auto p1 = &path[i];
            auto p2 = &path[j];

            int d = not_in_common_distance(*p1,*p2);

            max_distance = max(max_distance,d);
        }
    }

    cout << max_distance;

    cout << visited;
    cout << distance;
    cout << path;
    cout << graph.nodes;
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
