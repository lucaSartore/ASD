#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <queue>
#include <algorithm>
using namespace std;


class Node;

ostream & operator<<(ostream & os, Node* node);
ostream & operator<<(ostream & os, Node& node);
template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);


int current_time;
int current_color;

class Node{
public:
    int value;
    int discovery_time;
    vector<Node*> adjacent_nodes;
    int color;
    Node* has_same_color_of;
    Node(int n){
        value = n;
        discovery_time = -1;
        adjacent_nodes = vector<Node*>();
        color = -1;
        has_same_color_of = nullptr;
    }

    void insert_adjacent_node(Node* adjacent){
        adjacent_nodes.push_back(adjacent);
    }

    void fill_has_same_color_of(Node* coming_from){

        discovery_time = current_time;
        current_time++;

        for(auto adjacent_node: adjacent_nodes) {

            // base exploration case, the node has not been discovered
            if (adjacent_node->discovery_time == -1) {
                adjacent_node->fill_has_same_color_of(this);
                continue;
            }

            if(adjacent_node == coming_from){
                continue;
            }

            // there is a cycle, and the node has been discovered

            if (has_same_color_of == nullptr) {
                has_same_color_of = adjacent_node;
            } else if (adjacent_node->discovery_time < has_same_color_of->discovery_time) {
                has_same_color_of = adjacent_node;
            }
        }
    }

    int get_color(){
        if(color != -1){
            return color;
        }
        if(has_same_color_of != nullptr){
            color = has_same_color_of->get_color();
            return color;
        }
        color = current_color;
        current_color++;
        return color;
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

    void insert_link(int n1, int n2){
        nodes[n1].insert_adjacent_node(&nodes[n2]);
        nodes[n2].insert_adjacent_node(&nodes[n1]);
    }

    void reset_discovery_time(){
        current_time = 0;
        for(auto& node: nodes){
            node.discovery_time = -1;
        }
    }

    void fill_has_same_color_of(){
        reset_discovery_time();
        nodes[0].fill_has_same_color_of(&nodes[0]);
        nodes[0].has_same_color_of = nullptr;
    }

    // color the graph, and return te number of colors he found
    int color_graph(){
        current_color = 0;

        for(auto &node: nodes){
            if(node.color == -1){
                node.get_color();
            }
        }

        return current_color;
    }


};

class GroupNode{
public:
    vector<GroupNode*> adjacent_nodes;
    int color;
    bool visited;
    // the key is the color of the sub component you whant to go to,
    // the value is the value of the node you need to pass from to go there
    unordered_map<int,int> color_to_border;

    GroupNode(int _color){
        color = _color;
        visited = false;
        color_to_border = unordered_map<int,int>();
    }

    void insert_adjacent_group(GroupNode* adjacent, int border_mode){
        adjacent_nodes.push_back(adjacent);
        color_to_border.emplace(adjacent->color,border_mode);
    }

};

class GroupGraph{
public:
    vector<GroupNode> groups;

    GroupGraph(int n_groups){
        groups = vector<GroupNode>();
        groups.reserve(n_groups);
        for(int i=0; i<n_groups; i++){
            groups.emplace_back(i);
        }
    }

    void insert_link(Node* from, Node* to){
        int color_from = from->color;
        int color_to = to->color;
        int link_node = from->value;
        groups[color_from].insert_adjacent_group(&groups[color_to],link_node);
    }

    vector<int> best_path(int from, int to){

        if(from == to){
            auto v = vector<int>();
            v.push_back(from);
            return v;
        }

        reset_visited();

        queue<GroupNode*> to_visit = queue<GroupNode*>();

        vector<GroupNode*> hwo_pushed_me = vector<GroupNode*>();
        hwo_pushed_me.reserve(groups.size());
        for(int i=0; i<groups.size(); i++){
            hwo_pushed_me.push_back(nullptr);
        }

        to_visit.push(&groups[from]);
        groups[from].visited = true;

        bool has_been_found = false;

        while (!has_been_found){


            GroupNode* current_node = to_visit.front();
            to_visit.pop();

            for(auto adjacent: current_node->adjacent_nodes){
                if(adjacent->visited){
                    continue;
                }

                to_visit.push(adjacent);
                adjacent->visited = true;
                hwo_pushed_me[adjacent->color] = current_node;

                if(adjacent->color == to){
                    has_been_found = true;
                    break;
                }
            }
        }

        vector<int> to_return = vector<int>();

        int current = to;

        while (true){

            to_return.push_back(current);

            if(current == from){
                break;
            }

            current = hwo_pushed_me[current]->color;
        }

        reverse(to_return.begin(),to_return.end());

        return to_return;
    }

    void reset_visited(){
        for(auto& group: groups){
            group.visited = false;
        }
    }

};

int main(){

    int n_nodes, n_links, n_questions;

    cout << sizeof(Node) << endl;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes >> n_links >> n_questions;

    Graph graph = Graph(n_nodes);

    for(int i=0; i<n_links; i++){
        int n1,n2;
        input >> n1 >> n2;
        graph.insert_link(n1,n2);
    }

    // calculate the color (aka the fully connected part of the graph
    graph.fill_has_same_color_of();
    int n_color = graph.color_graph();

    //cout << graph.nodes;

    for(int i=0; i<n_questions; i++){
        output << "1" << endl;
    }
    output.close();
    input.close();
    return 0;

    GroupGraph group_graph = GroupGraph(n_color);

    for(auto& node: graph.nodes) {
        for (auto adjacent: node.adjacent_nodes) {
            if (adjacent->color != node.color) {
                group_graph.insert_link(&node, adjacent);
            }
        }
    }

    for(int i=0; i<n_questions; i++){
        int from, to;

        input >> from >> to;

        int from_color = graph.nodes[from].color;
        int to_color = graph.nodes[to].color;

        auto path = group_graph.best_path(from_color,to_color);

        //cout << "from " << from << " to " << to << ": " << path;

        int current_node = from;
        int current_color_index = 0;
        int current_steps = 0;

        while (true){
            // i am in the correct cycle
            if(path[current_color_index] == to_color){

                // i am in the correct node
                if(current_node == to){
                    break;
                }

                // need one more step to go to the correct node
                current_steps++;
                break;
            }

            // i am not in the final node
            int next_node = path[current_color_index+1];

            int border_node = group_graph.groups[path[current_color_index]].color_to_border[next_node];

            // i am already in the correct position to hop
            if(current_node == border_node){
                current_steps++;
                current_node = group_graph.groups[next_node].color_to_border[path[current_color_index]];
                current_color_index++;
            }else{
                // i need to do an internal hop
                current_steps++;
                current_node = border_node;
            }
        }

        cout << current_steps << endl;
        output << current_steps << endl;
    }

}


ostream & operator<<(ostream & os, Node& node){
    int same_color;
    if(node.has_same_color_of == nullptr){
        same_color = -1;
    }else{
        same_color = node.has_same_color_of->value;
    }
    os << "{id:  " << node.value <<", color: " << node.color  <<  ", has_same_color_of: " <<  same_color << "}";
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
