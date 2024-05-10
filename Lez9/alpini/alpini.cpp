//
// Created by lucas on 5/10/2024.
//
//
// Created by lucas on 23/03/2024.
//
#ifndef LOCAL
#include "alpini.h"
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
using namespace std;

#define GENERATION_SIZE 100
#define MAX_N_ATTEMPTS 20
#define N_MUTATIONS 5
#define MAX_TOTAL_REATTEMPT 30000000000

class Node{
public:
    vector<Node*> adjacent;
    int id;
    explicit Node(int _id){
       id = _id;
       adjacent = vector<Node*>();
    }

    bool can_find_visited(Node* coming_from, vector<bool>& visited, vector<bool>& mask){
        visited[id] = true;
        for(auto adj: adjacent){
            if(!mask[adj->id] || adj == coming_from){
                continue;
            }
            if(visited[adj->id]){
                return true;
            }
            if(adj->can_find_visited(this,visited,mask)){
                return true;
            }
        }
        return false;
    }
};
class Graph{
public:
   vector<bool> visited;
   vector<Node> nodes;

   Graph(int n_nodes){
      visited = vector<bool>(n_nodes,false);
      nodes = vector<Node>();
      nodes.reserve(n_nodes);
      for(int i=0; i<n_nodes; i++){
          nodes.emplace_back(i);
      }
   }

   void add_link(int n1, int n2){
       nodes[n1].adjacent.push_back(&nodes[n2]);
       nodes[n2].adjacent.push_back(&nodes[n1]);
   }

   bool is_cilcic(vector<bool> mask){
       fill(visited.begin(),visited.end(),false);
       for(int i=0; i<visited.size(); i++){
           if(visited[i] || !mask[i]){
               continue;
           }
           if(nodes[i].can_find_visited(nullptr,visited,mask)){
               return true;
           }
       }
       return false;
   }
};

class Solution{
public:
    vector<bool> mask;

    Solution(int n){
        mask = vector<bool>(n,false);
    }

    void mutate(){
        if(rand()%2){
            add_mutate();
        } else{
            swap_mutate();
        }
    }

    void add_mutate(){
       int i = rand()%mask.size();
       mask[i] = true;
    }
    void swap_mutate(){

        int i = rand()%mask.size();
        int j = rand()%mask.size();
        bool tmp = mask[i];
        mask[i]=mask[j];
        mask[j]=tmp;
    }

    int count_nodes(){
        int c=0;
        for(auto v: mask){
            if(v){
                c++;
            }
        }
        return c;
    }

    int get_score(Graph &g){
        if(g.is_cilcic(mask)){
            return 0;
        }
        return count_nodes();
    }
};



int find_solution(ofstream& of, Graph& g, int absolute_best){
   Solution current_solution(g.nodes.size());
   Solution current_best = current_solution;
   int max_score = 0;
   int n_attempts = 0;
   while (n_attempts<MAX_N_ATTEMPTS){
       bool has_improve = false;
       for(int i=0; i<GENERATION_SIZE; i++){
          Solution to_mutate = current_solution;
          for(int j=0; j<N_MUTATIONS; j++){
              to_mutate.mutate();
          }
          int score = to_mutate.get_score(g);
          if(score > max_score){
              has_improve = true;
              max_score = score;
              current_best = to_mutate;
          }
       }
       if(!has_improve){
           n_attempts++;
           continue;
       }
       n_attempts = 0;

       if(max_score <= absolute_best){
           continue;
       }
       absolute_best = max_score;

       // num of closed cities
       of << g.nodes.size() - max_score << " ";
       current_solution = current_best;
       for(int i=0; i<current_solution.mask.size(); i++){
           if(!current_solution.mask[i]){
               of << i << " ";
           }
       };
       of << "#" << endl;
   }
    return absolute_best;
}

int main(){

    ifstream input("input.txt");
    ofstream output("output.txt");

    int n_nodes, n_endges;
    input >> n_nodes >> n_endges;

    Graph g = Graph(n_nodes);

    for(int i=0; i<n_endges;i++) {
        int n1,n2;
        input >> n1 >> n2;
        g.add_link(n1,n2);
    }

    int absolute_best = 0;
    for(int i=0;  i < MAX_TOTAL_REATTEMPT; i++){
        absolute_best = find_solution(output,g, absolute_best);
    }


    output.close();
    input.close();

    return 0;
}
