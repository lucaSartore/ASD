//
// Created by lucas on 23/03/2024.
//
//
// Created by lucas on 23/03/2024.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
using  namespace  std;

class FirstStageNode;

class Link{
public:
    FirstStageNode* node;
    int max_distance_from_portal;
    int distance;
};

class FirstStageNode{
public:
    int id;
    vector<Link> adjacent;
    bool is_portal;
    Link father;
    //the distance to the furthest portal going down the tree
    int portal_distance_down;
    //the distance to the furthest portal going up the tree
    int portal_distance_up;

    void insert_adjacent(FirstStageNode* node, int distance){
        adjacent.push_back(Link{node,0, distance});
    }
    explicit FirstStageNode(int _id){
        id = _id;
        is_portal = false;
        father = Link{nullptr,0,0};
        adjacent = vector<Link>();
    }
};

class FirstStageGraph{
public:
   vector<FirstStageNode> nodes;
   int root;

   explicit FirstStageGraph(int n_nodes){
        nodes = vector<FirstStageNode>();
        nodes.reserve(n_nodes);
        for(int i=0; i<n_nodes; i++){
            nodes.emplace_back(i);
        }
   }
   void insert_link(int n1, int n2, int cost){
       nodes[n1].insert_adjacent(&nodes[n2],cost);
       nodes[n2].insert_adjacent(&nodes[n1],cost);
   }
   void find_portals(){
      for(auto& node: nodes){
          if(node.adjacent.size() == 1){
              node.is_portal = true;
          }
      }
   }
   void fill_father(){
       vector<bool> visited = vector<bool>(nodes.size(), false);
       vector<bool> pushed = vector<bool>(nodes.size(), false);

        deque<FirstStageNode*> to_visit;

       for(auto& node: nodes){ if(node.is_portal){
               to_visit.push_back(&node);
               pushed[node.id] = true;
           }
       }

       while (!to_visit.empty()){
          auto node = to_visit.front();
          to_visit.pop_back();

          // the last node to be removed from the vector will be the
          root = node->id;

          visited[node->id] = true;

          for(auto adj: node->adjacent){
              if(visited[adj.node->id]){
                  continue;
              }
              node->father = adj;
              if(!pushed[adj.node->id]){
                  to_visit.push_back(adj.node);
              }
          }
       }
   }

   int fill_portal_distance_down(FirstStageNode* node) {

       if(node->is_portal){
           node->portal_distance_down = 0;
           return 0;
       }

       int max_distance = INT_MIN;

       for(auto& link: node->adjacent){
           int d = link.distance + fill_portal_distance_down(link.node);
           max_distance = max(max_distance,d);
       }

       node->portal_distance_down = max_distance;
       return max_distance;
   }

   void fill_portal_distance_up(){
       for(auto &node: nodes){

          if(node.father.node == nullptr){
              node.portal_distance_up = -1;
              continue;
          }

          int max_distance = 0;

          for(auto next_hop: node.father.node->adjacent){
              if(next_hop.node == &node){
                  continue;
              }
              max_distance = max(max_distance,next_hop.distance);
          }
          max_distance += node.father.distance;

          node.portal_distance_up = max_distance;
       }
   }

   void fill_portal_distance(){
       fill_portal_distance_down(&nodes[root]);
       fill_portal_distance_up();
   }
};

int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");
    int n;

    input >> n;

    auto g1 = FirstStageGraph(n);

    for(int i=0; i<n-1; i++){
        int p,a,w;
        input >> p >> a >> w;
        g1.insert_link(p,a,w);
    }

    g1.fill_father();
    g1.fill_portal_distance();

    input.close();
    output.close();
    return 0;
}
