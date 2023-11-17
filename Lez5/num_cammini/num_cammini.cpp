#include <istream>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <tuple>
using namespace std;

class Node;
class Graph;
void fill_distance_vector(Graph& graph, Node* from,vector<int>& distance_vector);

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


int distance(Graph& graph, Node* node_from, Node* node_to){
    vector<int> distance_vector = vector<int>();
    distance_vector.reserve(graph.nodes.size());
    for(int i=0; i<graph.nodes.size(); i++){
        distance_vector.push_back(0);
    }
    fill_distance_vector(graph, node_from, distance_vector);
    return distance_vector[node_to->value];
}

void fill_distance_vector(Graph& graph, Node* from,vector<int>& distance_vector){
    vector<bool> visited = vector<bool>();
    visited.reserve(graph.nodes.size());
    for(int i=0; i<graph.nodes.size(); i++){
        visited.push_back(false);
    }

    // breath first search
    queue<tuple<Node*,int>> order_visit = queue<tuple<Node*,int>>();
    visited[from->value] = true;
    order_visit.emplace(from,0);
    distance_vector[from->value] = 0;

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
            distance_vector[adjacent->value] = distance+1;
            order_visit.emplace(adjacent,distance+1);
        }
    }
}

int num_ways_to_reach(Graph& graph, Node* from,Node* to, int steps){

    vector<int> distance_vector = vector<int>();
    vector<int> num_ways_to_reach = vector<int>();
    vector<bool> has_been_pushed = vector<bool>();
    num_ways_to_reach.reserve(graph.nodes.size());
    distance_vector.reserve(graph.nodes.size());
    has_been_pushed.reserve(graph.nodes.size());

    for(int i=0; i<graph.nodes.size(); i++){
        num_ways_to_reach.push_back(0);
        distance_vector.push_back(0);
        has_been_pushed.push_back(false);
    }

    fill_distance_vector(graph,from,distance_vector);

    // breath first search
    queue<Node*> order_visit = queue<Node*>();
    order_visit.emplace(from);
    num_ways_to_reach[from->value] = 1;

    while (!order_visit.empty()){

        Node* next_node =  order_visit.front();
        order_visit.pop();

        for(auto adjacent: next_node->adjacent_nodes){
            if(distance_vector[adjacent->value] <= distance_vector[next_node->value]){
                continue;
            }
            if(distance_vector[adjacent->value] > steps){
                break;
            }
            num_ways_to_reach[adjacent->value] += num_ways_to_reach[next_node->value];

            if(!has_been_pushed[adjacent->value]){
                order_visit.push(adjacent);
                has_been_pushed[adjacent->value] = true;
            }
        }
    }

    return num_ways_to_reach[to->value];
}

int main(){
    int n_nodes;
    int n_edges;
    int node_from;
    int node_to;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_edges;
    input >> node_from;
    input >> node_to;

    Graph graph(n_nodes);

    for(int i=0; i<n_edges; i++){
        int from,to;
        input >> from;
        input >> to;
        graph.insert_edge(from,to);
        //graph.insert_double_edge(from,to);
    }

    int distance_n1_n2 = distance(graph, &graph.nodes[node_from], &graph.nodes[node_to]);
    int num_paths = num_ways_to_reach(graph, &graph.nodes[node_from], &graph.nodes[node_to], distance_n1_n2);


    cout << "Distance: " << distance_n1_n2 << endl;
    cout << "Num path: " << num_paths << endl;

    output << distance_n1_n2 << " " << num_paths;

    output.close();
    input.close();
}