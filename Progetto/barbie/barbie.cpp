#include <istream>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <tuple>
#include <unordered_set>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <cassert>

using namespace std;

class Node;

class NodeRef{
public:
    int priority;
    Node* node;

    NodeRef(int _priority, Node* _node){
        priority = _priority;
        node = _node;
    }

    bool operator<(const NodeRef& other) const{
        return priority < other.priority;
    }
    bool operator>(const NodeRef& other) const{
        return priority > other.priority;
    }
    bool operator>= (const NodeRef& other) const{
        return priority >= other.priority;
    }
    bool operator<= (const NodeRef& other) const{
        return priority <= other.priority;
    }
};

class ReachOption{
public:
    int n_hops;
    int base_cost;
    bool has_impostors;

    ReachOption(int _n_hops, int _base_cost, bool _has_impostors){
        n_hops = _n_hops;
        base_cost = _base_cost;
        has_impostors = _has_impostors;
    }

    // merge the two options, returns true if the options is ged
    bool merge(ReachOption* other){
        assert(n_hops == other->n_hops);

        if(other->base_cost > base_cost){
            return false;
        }
        if(other->base_cost == base_cost){
            has_impostors |= other->has_impostors;
            return true;
        }
        if(other->base_cost < base_cost){
            base_cost = other->base_cost;
            has_impostors = other->has_impostors;
            return true;
        }
    }
};

class ReachOptions{
public:
    vector<ReachOption> options;

    ReachOptions(){
        options = vector<ReachOption>();
    }

    // returns true if the option was added
    bool add_option(ReachOption option){
        if(options.empty()){
            options.push_back(option);
            return true;
        }
        ReachOption* last_option = &options.back();

        if(last_option->n_hops > option.n_hops){
            return false;
        }

        if(last_option->n_hops == option.n_hops){
            if(last_option->base_cost < option.base_cost){
                return false;
            }
            if(last_option->base_cost == option.base_cost){

            }
        }

    }


};

class Link{
public:
    int cost;
    Node* node;

    Link(Node* _node, int _cost ){
        node = _node;
        cost = _cost;
    }
};

class Node{
public:
    int value;
    vector<Link> adjacent_nodes;
    bool is_occupied;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Link>();
        is_occupied = false;
    }

    void insert_adjacent_node(Node* node_to_insert, int cost) {
        adjacent_nodes.emplace_back(node_to_insert,cost);
    }

};

class Distance{
public:
    int distance;
    int n_hops;
    bool has_impostors;
    Distance(int _distance, int _n_hops, bool _has_impostors){
        distance = _distance;
        n_hops = _n_hops;
        has_impostors = _has_impostors;
    }
};

class Graph{
public:
    vector<Node> nodes;
    int POS_BARBIE;
    int POS_ALGORITMIA;

    explicit Graph(int number_of_nodes, int _pos_barbie, int _pos_algoritmia){
        nodes = vector<Node>();
        nodes.reserve(number_of_nodes);
        for(int i=0; i<number_of_nodes; i++){
            nodes.emplace_back(i);
        }
        POS_BARBIE = _pos_barbie;
        POS_ALGORITMIA = _pos_algoritmia;
    }

    void insert_edge(int n1,int n2, int cost){
        nodes[n1].insert_adjacent_node(&nodes[n2],cost);
        nodes[n2].insert_adjacent_node(&nodes[n1],cost);
    }

    Node* get(int value){
        return &nodes[value];
    }

    /// cost: o(n+m)
    vector<int> get_min_path(int from, int to){
        auto queue = priority_queue<NodeRef>();
        vector<int> distances = vector<int>(nodes.size(),-1);

        queue.emplace(0,get(from));

        while(!queue.empty()){
            NodeRef node_ref = queue.top();
            queue.pop();
            Node* node = node_ref.node;
            int distance = node_ref.priority;
            if(distances[node->value] != -1){
                continue;
            }
            distances[node->value] = distance;

            if(node->value == to){
                break;
            }

            for(auto adjacent_node: node->adjacent_nodes){
                if(distances[adjacent_node.node->value] != -1){
                    continue;
                }
                queue.emplace(distance+adjacent_node.cost,adjacent_node.node);
            }
        }
        int min_distance = distances[to];
        assert(min_distance != -1);

        vector<int> path = vector<int>();

        int current_node = from;
        path.push_back(current_node);
        while(current_node!=to){

            for(auto adj: get(current_node)->adjacent_nodes){
                if(distances[adj.node->value] == min_distance-adj.cost){
                    current_node = adj.node->value;
                    path.push_back(current_node);
                    min_distance -= adj.cost;
                    break;
                }
            }

        }

        return path;
    }
};

int main(){
    int n_nodes;
    int n_edges;
    int n_occupied_nodes;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes >> n_edges;

    const int POS_BARBIE = 0;
    const int POS_ALGORITMIA = n_nodes-1;

    Graph graph(n_nodes,POS_BARBIE,POS_ALGORITMIA);

    for(int i=0; i<n_nodes; i++){
        int n1,n2,cost;
        input >> n1 >> n2 >> cost;
        graph.insert_edge(n1,n2,cost);
    }

    input >> n_occupied_nodes;

    for(int i=0; i<n_occupied_nodes; i++){
        int occupied_node;
        input >> occupied_node;
        graph.nodes[occupied_node].is_occupied = true;
    }

    int max_num_hops = graph.get_min_path(POS_BARBIE,POS_ALGORITMIA).size()-1;

    output.close();
    input.close();
}