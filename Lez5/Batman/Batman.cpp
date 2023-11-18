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
#include <execution>
#include <unordered_map>

using namespace std;

# define UNKNOWN -1

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
    unordered_map<Node*,int> counter_adjacent_nodes;
    unordered_map<Node*,int> counter_adjacent_nodes_flipped;
    int group;
    int discovery_time;
    int closing_time;
    int num_ways_to_reach;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Node*>();
        adjacent_nodes = vector<Node*>();
        group = UNKNOWN;
        discovery_time = UNKNOWN;
        closing_time = UNKNOWN;
        counter_adjacent_nodes = unordered_map<Node*,int>();
        counter_adjacent_nodes_flipped = unordered_map<Node*,int>();
        num_ways_to_reach = -1;
    }

    void insert_adjacent_node(Node* node_to_insert) {
        // try insert 1
        auto result = counter_adjacent_nodes.emplace(node_to_insert,1);
        // if it already exist increase it by one.
        if(!get<1>(result)){
            counter_adjacent_nodes[node_to_insert]++;
            return;
        }
        // insert the node
        adjacent_nodes.push_back(node_to_insert);
    }
    void  insert_adjacent_node_flipped(Node* node_to_insert) {
        // try insert 1
        auto result = counter_adjacent_nodes_flipped.emplace(node_to_insert,1);
        // if it already exist increase it by one.
        if(!get<1>(result)){
            counter_adjacent_nodes_flipped[node_to_insert]++;
            return;
        }
        // insert the node
        adjacent_nodes_flipped.push_back(node_to_insert);
    }

    int get_num_wats_to_reach(Node* node_to_reach){
        if(num_ways_to_reach != UNKNOWN){
            return num_ways_to_reach;
        }
        num_ways_to_reach = 0;
        for(auto adjacent: adjacent_nodes_flipped){
            num_ways_to_reach += adjacent->get_num_wats_to_reach(node_to_reach)*counter_adjacent_nodes_flipped[adjacent];
        }
        return num_ways_to_reach;
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
};

int current_time;

void add_timestamp_recursive(Graph& graph, Node* current_node, vector<Node*>& visit_order){
    current_node->discovery_time = current_time;
    current_time++;
    visit_order.push_back(current_node);
    for(auto adjacent: current_node->adjacent_nodes){
        if(adjacent->discovery_time == UNKNOWN){
            add_timestamp_recursive(graph, adjacent, visit_order);
        }
    }
    current_node->closing_time = current_time;
    current_time++;
}

vector<Node*> add_timestamp(Graph& graph){

    current_time = 0;

    vector<Node*> visit_order = vector<Node*>();
    visit_order.reserve(graph.nodes.size());

    for(auto& node: graph.nodes){
        if(node.discovery_time == UNKNOWN){;
            add_timestamp_recursive(graph, &node, visit_order);
        }
    }

    return visit_order;
}


void paint(Node* node_to_paint, Node* father_node, int color){
    // if node_to_paint not reachable from father_node, return
    if(node_to_paint->discovery_time < father_node->discovery_time ||
        node_to_paint->closing_time > father_node->closing_time
    ){
        return;
    }
    if(node_to_paint->group == color){
        return;
    }
    assert(node_to_paint->group == UNKNOWN);
    node_to_paint->group = color;

    for(auto adjacent_node: node_to_paint->adjacent_nodes_flipped){
        paint(adjacent_node,father_node,color);
    }
}

int divide_in_groups(Graph& graph, vector<Node*>& order_visit){

    int group = 0;

    for(auto node: order_visit){
        if(node->group != UNKNOWN){
            continue;
        }
        paint(node,node,group++);
    }

    return ++group;
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

    vector<Node*> visit_order = add_timestamp(graph);

    int num_group = divide_in_groups(graph,visit_order);

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

    group_graph.nodes[group_pos_start].num_ways_to_reach = 1;

    int ways_to_reach = group_graph.nodes[group_pos_end].get_num_wats_to_reach(&group_graph.nodes[group_pos_start]);

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
    os << "{id:  " << node.value <<", group: "<< node.group <<
    ", dt: " << node.discovery_time << ", ct: " << node.closing_time<< ", links: [";
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
