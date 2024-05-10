//
// Created by lucas on 23/03/2024.
//
#include "tsp.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <algorithm>
#include <random>
using namespace std;


#define SWAP_SINGLE_MUTATION_PROBABILITY 50
#define POPULATION_SIZE 400
#define ATTEMPTS 100000000000
#define BLOCK_REVERT_PROBABILITY 50
#define ALLOWED_REATTEMPTS 25
#define REORDER_LEN 6

auto re = default_random_engine(0);

vector<bool> visited;

void find_path(int& current_min, vector<vector<int>>& distances, int current_distance, int current_node, int size, vector<int> current_path, int count, ofstream& fs, vector<int> &nodes_to_visit){

    if(current_distance >= current_min){
        return;
    }

    current_path[count] = current_node;

    if(count+1 == size ){

        current_distance += distances[current_node][0];

        if(current_distance >= current_min){
            return;
        }

        current_min = current_distance;
    }

    visited[current_node] = true;

    for(int i: nodes_to_visit){
        if(visited[i]){
            continue;
        }
        find_path(current_min, distances, current_distance + distances[i][current_node], i, size,current_path,count+1, fs, nodes_to_visit);
    }

    visited[current_node] = false;
}
class Solution{
public:
    vector<int> cityes;

    Solution(int size){
       cityes = vector<int>(size,0);
       for(int i=0; i<size; i++){
           cityes[i] = i;
       }
        shuffle(cityes.begin(),cityes.end(),re);
    }

    void mutate(vector<vector<int>> g){
       if(rand()%101 <= SWAP_SINGLE_MUTATION_PROBABILITY){
            swap_single_mutation();
       }else{
          swap_block_mutation();
       }
    }

    void swap_single_mutation(){
        int index_1 = rand()%cityes.size();
        int index_2 = rand()%cityes.size();
        int tmp = cityes[index_1];
        cityes[index_1] = cityes[index_2];
        cityes[index_2] = tmp;
    }

    void swap_block_mutation(){

        bool revert = rand()%100 < BLOCK_REVERT_PROBABILITY;

        //revert = false;

        int swap_size = rand()%(cityes.size()/2-1);
        int first_index = rand()%cityes.size();
        int second_index_max_distance = cityes.size()-2*swap_size;
        int second_index = (first_index+swap_size+rand()%second_index_max_distance)%cityes.size();

        for(int i=0; i<swap_size; i++){
            int x1 = (first_index+i)%cityes.size();
            int x2;
            if(!revert){
                x2 = (second_index+i)%cityes.size();
            }else{
                x2 = (second_index + swap_size -1 -i + cityes.size())%cityes.size();
            }
            int tmp = cityes[x1];
            cityes[x1] = cityes[x2];
            cityes[x2] = tmp;
        }
    }

    int get_total_distance(vector<vector<int>>& distances){
        int c=0;
        for(int i=0; i<cityes.size(); i++){
            int j = (i+1)%cityes.size();
            int c1 = cityes[i];
            int c2 = cityes[j];
            c += distances[c1][c2];
        }
        return c;
    }

    void reorder(vector<vector<int>> g){
        int reorder_len = max(REORDER_LEN,(int)cityes.size());
        vector<int> to_reorder = vector<int>(reorder_len ,0);
        int reorder_start = rand()%cityes.size();
        for(int i=0; i<reorder_len; i++){
           int p = (reorder_start + i)%cityes.size();
           to_reorder[i]=cityes[p];
        }
        vector<int> reordered = vector<int>(reorder_len ,0);
        int current_min = INT_MAX;
        find_path(current_min,g,0,to_reorder[0],reorder_len,)
    }
};

Solution mutation_cycle(Solution current_solution, vector<vector<int>> & distances){

    int current_best = current_solution.get_total_distance(distances);
    Solution next_best = Solution(0);


    for(int i=0; i<POPULATION_SIZE; i++){
        Solution mutated = current_solution;
        mutated.mutate();
        int mutated_score = mutated.get_total_distance(distances);
        if(mutated_score < current_best){
            current_best = mutated_score;
            next_best = mutated;
            cout<< current_best << endl;
        }
    }
    return next_best;
}

int main(){

    ifstream input("input.txt");
    ofstream output("output.txt");

    int size;
    input >> size;


    vector<vector<int>> distances = vector<vector<int>>();
    distances.reserve(size);

    for(int i=0; i<size;i++) {
        distances.emplace_back(size, 0);
    }

    for(int y=1; y<size; y++){
        for(int x=0; x<y; x++){
            int v;
            input >> v;
            distances[x][y] = v;
            distances[y][x] = v;
        }
    }

    int current_best = INT_MAX;

    for(int i=0; i<ATTEMPTS; i++){

        Solution solution(size);
        int current_reattempts = 0;
        while (true){

            Solution new_solution = mutation_cycle(solution,distances);
            if(new_solution.cityes.empty()){
                current_reattempts++;
                if(current_reattempts>ALLOWED_REATTEMPTS) {
                    break;
                }else{
                    continue;
                }
            }
            solution = new_solution;
        }

        int new_cost = solution.get_total_distance(distances);

        if(new_cost > current_best){
            continue;
        }
        current_best = new_cost;
        for(auto e: solution.cityes){
            output << e << " ";
        }
        output << solution.cityes[0] << " #" << endl;

    }


    output.close();
    input.close();

    return 0;
}
