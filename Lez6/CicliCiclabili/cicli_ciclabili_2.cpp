#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <queue>
#include <algorithm>
using namespace std;



class Node;
class GroupNode;

ostream & operator<<(ostream & os, Node* node);
ostream & operator<<(ostream & os, Node& node);
template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);
ostream & operator<<(ostream & os, GroupNode* node);
ostream & operator<<(ostream & os, GroupNode& node);


class Node{
public:
    int value;
    vector<Node*> adjacent_nodes;
    int distance_from_root;
    Node* father_intersection;
    Node* father;
    bool is_root;
    Node(int n){
        value = n;
        adjacent_nodes = vector<Node*>();
        distance_from_root = -1;
        father_intersection = nullptr;
        is_root = false;
    }

    void insert_adjacent_node(Node* adjacent){
        adjacent_nodes.push_back(adjacent);
    }

    // this node is an itersection if it hase more that 3 adjacent nodes
    bool is_intersection() {
        return adjacent_nodes.size() >= 3 || is_root;
    }

    void propagate_father_intersection(Node* new_father_intersection, Node* new_father) {
        father_intersection = new_father_intersection;
        father = new_father;

        new_father = this;
        if (is_intersection()) {
            new_father_intersection = this;
        }

        for (auto adjacent: adjacent_nodes) {
            // skip the father
            if (adjacent->distance_from_root <= distance_from_root) {
                continue;
            }
            adjacent->propagate_father_intersection(new_father_intersection,new_father);
        }
    }
};


class Graph{
public:
    vector<Node> nodes;
    int root;

    Graph(int n_nodes, int _root = 0){
        nodes = vector<Node>();
        root = _root;
        nodes.reserve(n_nodes);
        for(int i=0; i<n_nodes; i++){
            nodes.emplace_back(i);
        }
        nodes[root].is_root = true;
    }

    void insert_link(int n1, int n2){
        nodes[n1].insert_adjacent_node(&nodes[n2]);
        nodes[n2].insert_adjacent_node(&nodes[n1]);
    }

    void propagate_distance_from_root(){
        nodes[root].distance_from_root = 0;

        queue<Node*> to_visit = queue<Node*>();
        to_visit.push(&nodes[root]);

        while (!to_visit.empty()){

            Node* node = to_visit.front();
            to_visit.pop();
            for(auto adjacent: node->adjacent_nodes){
                if(adjacent->distance_from_root != -1){
                    continue;
                }
                adjacent->distance_from_root = node->distance_from_root + 1;
                to_visit.push(adjacent);
            }
        }
    }


    void fill_father_intersection(){
        nodes[root].propagate_father_intersection(nullptr, nullptr);
    }

};


bool are_adjacent(Node* n1, Node* n2){
    for(auto adj: n1->adjacent_nodes){
        if(adj == n2){
            return true;
        }
    }
    return false;
}

int distance_2_nodes(Node* n1, Node* n2){
    Node* previus_parent_n1 = n1;
    Node* previus_parent_n2 = n2;
    Node* parent_n1 = n1;
    Node* parent_n2 = n2;

    while (parent_n1!=parent_n2){
        if(parent_n1->distance_from_root > parent_n2->distance_from_root){
            previus_parent_n1 = parent_n1;
            parent_n1 = parent_n1->father_intersection;
        }else{
            previus_parent_n2 = parent_n2;
            parent_n2 = parent_n2->father_intersection;
        }
    }


    bool possible_bad_condition = false;
    if(previus_parent_n1 == n1 && previus_parent_n2->distance_from_root > n1->distance_from_root){
        possible_bad_condition = true;
    }
    if(previus_parent_n2 == n2 && previus_parent_n1->distance_from_root > n2->distance_from_root){
        possible_bad_condition = true;
    }
    if(possible_bad_condition){
        parent_n2 = previus_parent_n2;
        parent_n1 = previus_parent_n1;
        while (parent_n1!=parent_n2){
            if(parent_n1->distance_from_root > parent_n2->distance_from_root){
                previus_parent_n1 = parent_n1;
                parent_n1 = parent_n1->father;
            }else{
                previus_parent_n2 = parent_n2;
                parent_n2 = parent_n2->father;
            }
        }
    }

    if(parent_n1 == n1){
        return n2->distance_from_root - n1->distance_from_root;
    }
    if(parent_n2 == n2){
        return n1->distance_from_root - n2->distance_from_root;
    }


    int distance = n1->distance_from_root + n2->distance_from_root - 2*parent_n2->distance_from_root;
    if(are_adjacent(previus_parent_n1,previus_parent_n2)){
        distance--;
    }
    return distance;
}


// calculate the distance between to node, (each one madke part of a different group, where node 2 is an ancestor of node 1
int distance_linear(Node* node_from, Node* node_to){
    return 0;
}

// calculate the distance between to nodes, that are not one the ancestor of the other, but have a common ancestor
int distance_on_tree(Node* n1, Node* n2, Node* common_ancestor){
    return 0;
}


int calculate_distance_2(Graph& graph,Node* n1, Node* n2){

    if(n1 == n2){
        return 0;
    }

    queue<int> distances = queue<int>();
    queue<Node*> to_visit = queue<Node*>();

    vector<bool> is_pushed = vector<bool>();
    is_pushed.reserve(graph.nodes.size());

    for(int i=0; i<graph.nodes.size(); i++){
        is_pushed.push_back(false);
    }

    to_visit.push(n1);
    distances.push(0);
    is_pushed[n1->value] = true;

    while (!to_visit.empty()){

        Node* node = to_visit.front();
        to_visit.pop();
        int distance = distances.front();
        distances.pop();

        for(auto adj: node->adjacent_nodes){
            if(is_pushed[adj->value]){
                continue;
            }
            if(adj == n2){
                return distance+1;
            }
            to_visit.push(adj);
            distances.push(distance+1);
            is_pushed[adj->value] = true;
        }

    }
    assert(false);
    return -1;
}

int main(){

    int n_nodes, n_links, n_questions;


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
    graph.propagate_distance_from_root();
    graph.fill_father_intersection();

    //cout << graph.nodes << endl;


    for(int i=0; i<n_questions; i++){
        int from,to;
        input >> from >> to;

        //if(from != 3043 || to != 3143){
        //      continue;
        //}

        Node* node_from = &graph.nodes[from];
        Node* node_to = &graph.nodes[to];

        int distance = distance_2_nodes(node_from,node_to);

        /*
        if(parent == node_from || parent == node_to){
            distance = distance_linear(node_from,node_to);
        }else{
            distance = distance_on_tree(node_to,node_from,parent);
        }*/

        //int actual_distance = calculate_distance_2(graph,node_to,node_from);
        //cout << "from " << from << " to: " << to << ": " << distance << " " << actual_distance << endl;
        //assert(distance == actual_distance);

        output << distance << endl;
    }

    output.close();
    input.close();

    return 0;
}


ostream & operator<<(ostream & os, Node& node){
    int father_intersection = -1;
    if(node.father_intersection!= nullptr){
        father_intersection = node.father_intersection->value;
    }
    os << "{id:  " << node.value << ", distance: " << node.distance_from_root << ", father intersection: " << father_intersection << "}";
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