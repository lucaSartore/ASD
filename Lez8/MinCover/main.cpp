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

    explicit Tree(int _n){
        n = _n;
        nodes = vector<Node>();
        nodes.reserve(n);
        for(int i=0; i<n; i++){
            nodes.push_back(Node(i));
        }
    }

    void insert_arch(int father, int child){
        nodes[father].AddChild(&nodes[child]);
    }
};

bool min_cover(Node* node, int * count){
    bool need_to_set = false;
    for(auto c: node->childs){
        if(!min_cover(c,count)){
            need_to_set = true;
        }
    }
    if(need_to_set){
        (*count)++;
        return true;
    }
    return false;
}

int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    int n;
    input >> n;

    Tree tree = Tree(n);

    for(int i=0; i<n-1; i++){
       int f,c;
       input >> f >> c;
       tree.insert_arch(f,c);
    }
    int count = 0;
    min_cover(&tree.nodes[0],&count);

    output << count;

    input.close();
    output.close();
    return 0;
}
