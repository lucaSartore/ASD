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
    int max_distance_portal;
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
          to_visit.pop_front();

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
           if(link.node == node->father.node){
               continue;
           }
           int d = link.distance + fill_portal_distance_down(link.node);
           max_distance = max(max_distance,d);
       }

       node->portal_distance_down = max_distance;
       return max_distance;
   }

   void fill_portal_distance_up(FirstStageNode* node){

        if(node->father.node == nullptr){
            node->portal_distance_up = -1;
        }else{

           int max_distance = node->father.node->portal_distance_up;

           for(auto & next_hop: node->father.node->adjacent){
               // can't go back, otherwise it won't be a proper "road"
               if(next_hop.node == node || next_hop.node == node->father.node->father.node){
                   continue;
               }

               max_distance = max(max_distance,next_hop.node->portal_distance_down + next_hop.distance);
           }

           max_distance += node->father.distance;

           node->portal_distance_up = max_distance;
        }

        for(auto& adj: node->adjacent){
            if(adj.node == node->father.node){
                continue;
            }
            fill_portal_distance_up(adj.node);
        }

   }

   void fill_max_distance_portal(){
        for(auto& node: nodes){
            node.max_distance_portal = max(node.portal_distance_up,node.portal_distance_down);
        }
   }

   void fill_portal_distance(){
       fill_portal_distance_down(&nodes[root]);
       fill_portal_distance_up(&nodes[root]);
   }
};



class SecondStageNode;
class Link2{
public:
    SecondStageNode* node;
    int distance;
};

class SecondStageNode{
public:
    int id;
    vector<Link2> adjacent;
    Link2 father;
    void insert_father(SecondStageNode* node, int distance){
        father = Link2{node,distance};
    }
    void insert_adjacent(SecondStageNode* node, int distance){
        adjacent.push_back(Link2{node, distance});
    }
    explicit SecondStageNode(int _id){
        id = _id;
        father = Link2{nullptr,0};
        adjacent = vector<Link2>();
    }
};

class SecondStageGraph{
public:
    vector<SecondStageNode> nodes;
    int root;

    explicit SecondStageGraph(int n_nodes){
        nodes = vector<SecondStageNode>();
        nodes.reserve(n_nodes);
        for(int i=0; i<n_nodes; i++){
            nodes.emplace_back(i);
        }
    }
    void insert_link(int father, int child, int cost){
        nodes[father].insert_adjacent(&nodes[child],cost);
        nodes[child].insert_adjacent(&nodes[father],cost);
        nodes[child].insert_father(&nodes[father],cost);
    }
};

void transpose_graph_r(FirstStageNode* node, SecondStageGraph& g){

    for(auto &adj: node->adjacent){
        if(adj.node == node->father.node){
            continue;
        }
        g.insert_link()
    }


}

SecondStageGraph transpose_graph(FirstStageGraph & graph){
    SecondStageGraph g = SecondStageGraph(graph.nodes.size());




}

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

    g1.find_portals();
    g1.fill_father();
    g1.fill_portal_distance();
    g1.fill_max_distance_portal();
    input.close();
    output.close();
    return 0;
}
