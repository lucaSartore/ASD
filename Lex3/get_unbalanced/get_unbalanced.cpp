#include <iostream>
using namespace std;

class Node{
public:
    int value;
    Node* left;
    Node* right;

    Node(int _value, Node* _left, Node* _right){
        value = _value;
        left = _left;
        right = _right;
    }
};

class Return{
public:
    int sub_nodes;
    int unbalanced;

    Return(int _sub_nodes, int _unbalanced){
        sub_nodes = _sub_nodes;
        unbalanced =_unbalanced;
    }
};

Return get_unbalanced(Node* node){
    if(node == nullptr){
        return Return(0,0);
    }
    auto r1 = get_unbalanced(node->right);
    auto r2 = get_unbalanced(node->left);
    

    int unbalanced = abs(r1.sub_nodes-r2.sub_nodes);
    unbalanced = max(unbalanced,r1.unbalanced);
    unbalanced = max(unbalanced,r2.unbalanced);

    int sub_nodes = r1.sub_nodes + r2.sub_nodes + 1;

    return Return(sub_nodes,unbalanced);
}

int main(){

    auto left_left = Node(0, nullptr, nullptr);
    auto left_right = Node(0, nullptr, nullptr);
    auto left = Node(0,&left_left,&left_right);

    auto right_right_right = Node(20, nullptr, nullptr);
    auto right_right = Node(21, nullptr, &right_right_right);
    auto right = Node(22, nullptr,&right_right);

    auto root = Node(0,&left,&right);

    cout << "Unbalanced: " << get_unbalanced(&root).unbalanced << endl;

}