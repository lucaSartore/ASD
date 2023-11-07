#include <iostream>
#include <vector>

using namespace std;

class Node{
public:
    // distance = -1 equal not reachable.
    int distance_from_n1;
    int distance_from_n2;
    int value;
    vector<Node*> adjacent_nodes;

    Node(int _value){
        value = _value;
        distance_from_n1 = -1;
        distance_from_n2 = -1;
        adjacent_nodes = vector<Node*>();
    }

    void insert_adjacent_node(Node* node_to_insert){
        // optional check the node is not already in the vector
        adjacent_nodes.push_back(node_to_insert);
    }

    // propagate the distance from n1 and n2
    void propagate_distance(){
        for(auto adjacent_node: adjacent_nodes)
        {
            // propagate n1
            if(distance_from_n1 != -1){
                if( adjacent_node->distance_from_n1 > distance_from_n1+1 ||
                    adjacent_node->distance_from_n1 == -1){
                    adjacent_node->distance_from_n1 = distance_from_n1+1;
                }
            }
            // propagate n2
            if(distance_from_n2 != -1){
                if( adjacent_node->distance_from_n2 > distance_from_n2+1 ||
                    adjacent_node->distance_from_n2 == -1){
                    adjacent_node->distance_from_n2 = distance_from_n2+1;
                }
            }

            adjacent_node->propagate_distance();
        }
    }
};

int count_nodes_same_distance(Node* n1, Node* n2, vector<Node>& nodes){
    // when reaching this pont all nodes have
    // distance from n2 and n2 set to -1
    n1->distance_from_n1 = 0;
    n2->distance_from_n2 = 0;

    n1->propagate_distance();
    n2->propagate_distance();

    int count = 0;

    for(Node& node: nodes){
        if(node.distance_from_n2 == node.distance_from_n1 && node.distance_from_n1 != -1){
            count++;
        }
    }

    return count;
}

int main(){

    vector<Node> nodes = vector<Node>();

    // create 10 nodes
    for(int i=0; i<10; i++){
        nodes.emplace_back(i);
    }


    nodes[0].insert_adjacent_node(&nodes[2]);
    nodes[2].insert_adjacent_node(&nodes[4]);

    nodes[1].insert_adjacent_node(&nodes[3]);
    nodes[3].insert_adjacent_node(&nodes[4]);

    nodes[4].insert_adjacent_node(&nodes[5]);

    nodes[0].insert_adjacent_node(&nodes[7]);
    nodes[1].insert_adjacent_node(&nodes[7]);
    // 0 is n1
    // 1 is n2
    int n = count_nodes_same_distance(&nodes[0],&nodes[1],nodes);

    cout << "number is: " << n << endl;
}