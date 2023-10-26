//
// Created by lucas on 24/10/2023.
//
#include <iostream>
using namespace std;

class Node{
public:
    int value;
    Node* left;
    Node* right;
};

int num_nodes_with_layer(Node* root, int current_level=0, int current_count=0){
    if(root->value == current_count){
        current_count++;
    }
    if(root->left != nullptr){
        current_count = num_nodes_with_layer(root->left,current_level+1,current_count);
    }
    if(root->right != nullptr){
        current_count = num_nodes_with_layer(root->right,current_level+1,current_count);
    }
    return current_count;
}

int main(){
}