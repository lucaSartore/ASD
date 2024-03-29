#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <climits>
#include <queue>
using  namespace  std;

class FirstStageNode;

class Link{
public:
    FirstStageNode* node;
    int distance;
};


class FirstStageNode{
public:
    int id;
    vector<Link> adjacent;
    int distance_closest_portal;
    int distance_furthest_portal;
    void insert_adjacent(FirstStageNode* node, int distance){
        adjacent.push_back(Link{node, distance});
    }
    void delete_adjacent(FirstStageNode* node){
        for(auto i = adjacent.begin(); i != adjacent.end(); i++){
            if(i->node == node){
                adjacent.erase(i);
                break;
            }
        }
    }
    explicit FirstStageNode(int _id){
        id = _id;
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
   class InQueueObject{
   public:
       int priority;
       FirstStageNode* node;
       bool operator<(InQueueObject const &other) const{
           return priority > other.priority;
       }
   };

   void fill_distance_closest_portal() {

        priority_queue<InQueueObject> to_visit = priority_queue<InQueueObject>();

       for(auto& node: nodes){
           if(node.adjacent.size() == 1){
               to_visit.push(InQueueObject{0,&node});
           }
           node.distance_closest_portal = -1;
       }

       while (!to_visit.empty()){

           auto node = to_visit.top();
           to_visit.pop();

           if(node.node->distance_closest_portal != -1 && node.node->distance_closest_portal < node.priority){
               // I already popped this node before, so i don't need to update it again
               continue;
           }

           node.node->distance_closest_portal= node.priority;

           for(auto& adj: node.node->adjacent){
               if(adj.node->distance_closest_portal == -1){
                   to_visit.push(InQueueObject{node.node->distance_closest_portal + adj.distance, adj.node});
               }
           }
       }
   }

    void remove_father_pointers_r(FirstStageNode* father){
       for(auto& node: father->adjacent){
           node.node->delete_adjacent(father);
           remove_father_pointers_r(node.node);
       }
    }
   void remove_father_pointers(){
      int father_id = -1;
      int max_distance_closest_portal = 0;
      int  current_id = 0;
      for(auto& node: nodes){
          if(node.distance_closest_portal > max_distance_closest_portal){
              max_distance_closest_portal = node.distance_closest_portal;
              father_id = current_id;
          }
          current_id++;
      }
       this->root = father_id;
       remove_father_pointers_r(&nodes[father_id]);
   }

   int get_distance_furthest_portal_down_the_tree(FirstStageNode* node){
       int max_v = 0;
       for(auto&node: node->adjacent){
           max_v = max(max_v, get_distance_furthest_portal_down_the_tree(node.node)+node.distance);
       }
       return max_v;
   }

   void fill_distance_furthest_portal(FirstStageNode* node){
        for(auto& adj: node->adjacent){
            adj.node->distance_furthest_portal = node->distance_furthest_portal + adj.distance;
            fill_distance_furthest_portal(adj.node);
        }
   }

   void fill_portal_distances(){
       fill_distance_closest_portal();
       remove_father_pointers();
       int max_d = get_distance_furthest_portal_down_the_tree(&nodes[root]);
       nodes[root].distance_furthest_portal = max_d;
       fill_distance_furthest_portal(&nodes[root]);
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
    void insert_adjacent(SecondStageNode* node, int distance){
        adjacent.push_back(Link2{node, distance});
    }
    explicit SecondStageNode(int _id){
        id = _id;
        adjacent = vector<Link2>();
    }
    int count_reachable_town_in_distance(int distance){
       if(distance == 0){
           return 1;
       }
       if(distance < 0){
           return 0;
       }
       int reachable = 1;
       for(auto &adj: adjacent){
           reachable += adj.node->count_reachable_town_in_distance(distance-adj.distance);
       }
        return reachable;
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
    }

    int get_max_n(int distance){
        int max_n = 0;
        for(auto& node: nodes){
            max_n = max(max_n,node.count_reachable_town_in_distance(distance));
        }
        return max_n;
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

    g1.fill_portal_distances();


    input.close();
    output.close();
    return 0;
}
