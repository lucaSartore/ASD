//
// Created by lucas on 23/03/2024.
//
//
// Created by lucas on 23/03/2024.
//
#include <iostream>
#include <fstream>
#include <vector>
using  namespace  std;

class Node{
public:
    vector<Node*> childs;
    int value;
    void AddChild(Node* child){
       childs.push_back(child);
    }
    explicit  Node(int _value){
        value = _value;
        childs = vector<Node*>();
    }
};

class Tree{
public:
    int n;
    vector<Node> nodes;

    explicit Tree(){
        n = 0;
        nodes = vector<Node>();
    }

    void add_node(int w){
        nodes.push_back(Node(w));
        n++;
    }

    void insert_arch(int father, int child){
        nodes[father].AddChild(&nodes[child]);
    }
};

struct Result{
    int sum_if_take;
    int sum_if_not_take;
};

Result min_cover(Node* node){
    int sum_if_take = node->value;
    int sum_if_not_take = 0;
    for(auto c: node->childs){
        Result r = min_cover(c);
        sum_if_take += min(r.sum_if_not_take,r.sum_if_take);
        sum_if_not_take += r.sum_if_take;
    }
    return Result{sum_if_take,sum_if_not_take};
}

int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    int n;
    input >> n;

    Tree tree = Tree();

    for(int i=0; i<n; i++){
        int w;
        input >> w;
        tree.add_node(w);
    }

    for(int i=0; i<n-1; i++){
       int f,c;
       input >> f >> c;
       tree.insert_arch(f,c);
    }
    Result r = min_cover(&tree.nodes[0]);

    output << min(r.sum_if_take,r.sum_if_not_take);

    input.close();
    output.close();
    return 0;
}
