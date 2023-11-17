#include <istream>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <tuple>
#include <unordered_set>
#include <algorithm>
#include <assert.h>
#include <set>
#include <unordered_map>

using namespace std;

# define UNKNOWN -1
# define UNREACHABLE -1

class Node;

ostream & operator<<(ostream & os, Node* node);
ostream & operator<<(ostream & os, Node& node);
ostream & operator<<(ostream & os, list<Node*>& path);
template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);
template<typename T>
ostream & operator<<(ostream & os, list<T>& v);

class Node{
public:
    int value;
    vector<Node*> adjacent_nodes;
    vector<Node*> adjacent_nodes_flipped;
    int group;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Node*>();
        adjacent_nodes = vector<Node*>();
        group = UNKNOWN;
    }

    void insert_adjacent_node(Node* node_to_insert) {
        adjacent_nodes.push_back(node_to_insert);
    }
    void  insert_adjacent_node_flipped(Node* node_to_insert) {
        adjacent_nodes_flipped.push_back(node_to_insert);
    }

    int num_ways_to_reach_inverse(int start_node){
        if(start_node == value){
            return 1;
        }
        int to_return = 0;
        for(auto node: adjacent_nodes_flipped){
            to_return+= node->num_ways_to_reach_inverse(start_node);
        }
        return to_return;
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
        nodes[to].insert_adjacent_node_flipped(&nodes[from]);
    }

    bool is_edge_present(int from, int to){
        return find(nodes[from].adjacent_nodes.begin(),
             nodes[from].adjacent_nodes.end(),
             &nodes[to]
             ) != nodes[from].adjacent_nodes.end();
    }

};

enum Direction{STRAIGHT,FLIPPED};

vector<bool> get_reachability_vector(Graph& graph, Node* start_node, Direction direction){

    vector<bool> reachability_vector = vector<bool>();
    vector<bool> has_been_pushed = vector<bool>();
    reachability_vector.reserve(graph.nodes.size());
    has_been_pushed.reserve(graph.nodes.size());
    for(int i=0; i<graph.nodes.size(); i++){
        reachability_vector.push_back(false);
        has_been_pushed.push_back(false);
    }

    queue<Node*> to_visit = queue<Node*>();
    to_visit.push(start_node);
    has_been_pushed[start_node->value] = true;

    while (!to_visit.empty()){

        auto next_node = to_visit.front();
        to_visit.pop();

        reachability_vector[next_node->value] = true;

        vector<Node*>* adjacent_nodes;
        if(direction == Direction::FLIPPED){
            adjacent_nodes = &next_node->adjacent_nodes_flipped;
        }else{
            adjacent_nodes = &next_node->adjacent_nodes;
        }

        for(auto adjacent_node: *adjacent_nodes){
            if(has_been_pushed[adjacent_node->value]){
                continue;
            }
            if(adjacent_node->group!=UNKNOWN){
                continue;
            }
            has_been_pushed[adjacent_node->value] = true;
            to_visit.push(adjacent_node);
        }
    }

    return reachability_vector;
}

int divide_in_groups(Graph& graph) {

    int current_group_id = 0;

    const int size = graph.nodes.size();

    for (int i = 0; i < size; i++) {

        Node *node = &graph.nodes[i];

        if (node->group != UNKNOWN) {
            continue;
        }

        auto reachable = get_reachability_vector(graph, node, Direction::STRAIGHT);
        auto reachable_flipped = get_reachability_vector(graph, node, Direction::FLIPPED);

        for (int j = 0; j < size; j++) {
            if (reachable[j] && reachable_flipped[j]) {
                graph.nodes[j].group = current_group_id;
            }
        }

        current_group_id++;
    }

    return current_group_id + 1;
}


int main(){
    int n_nodes;
    int n_edges;
    int pos_start;
    int pos_end;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes;
    input >> n_edges;
    input >> pos_start;
    input >> pos_end;

    Graph graph(n_nodes);

    for(int i=0; i<n_edges; i++){
        int from,to;
        input >> from;
        input >> to;
        graph.insert_edge(from,to);
    }

    int num_group = divide_in_groups(graph);

    int group_pos_start = graph.nodes[pos_start].group;
    int group_pos_end = graph.nodes[pos_end].group;

    Graph group_graph = Graph(num_group);

    // build the connection in the group graph
    for(const auto& node: graph.nodes){
        for(auto adjacent_node: node.adjacent_nodes){
            if(node.group != adjacent_node->group){
                //if(!group_graph.is_edge_present(node.group,adjacent_node->group)){
                    group_graph.insert_edge(node.group,adjacent_node->group);
                //}
            }
        }
    }

    int ways_to_reach = group_graph.nodes[group_pos_end].num_ways_to_reach_inverse(group_pos_start);

    /*
    cout << "ways_to_reach: " << ways_to_reach << endl;
    cout << "start_pos: " << group_pos_start << endl;
    cout << "end_pos: " << group_pos_end << endl;
    cout << graph.nodes;
    cout << group_graph.nodes;
    */

    output << ways_to_reach;

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

ostream & operator<<(ostream & os, Node& node){
    os << "{id:  " << node.value <<", group: "<< node.group << ", links: [";
    for(auto l: node.adjacent_nodes){
        os << l->value << ", ";
    }
    os << "]}";
    return os;
}

ostream & operator<<(ostream & os, Node* node){
    os << *node;
    return os;
}


template<typename T>
ostream & operator<<(ostream & os, vector<T>& v){
    os << "[" << endl;
    int c = 0;
    for(auto node: v){
        os << "\t" << c <<": " << node << "\n";
        c++;
    }
    os << "]\n";
    return os;
}

template<typename T>
ostream & operator<<(ostream & os, list<T>& v){
    os << "[" << endl;
    int c = 0;
    for(auto node: v){
        os << "\t" << c <<": " << node << "\n";
        c++;
    }
    os << "]\n";
    return os;
}
