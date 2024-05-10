//
// Created by lucas on 23/03/2024.
//
//#include "tsp.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
using namespace std;


#define SWAP_SINGLE_MUTATION_PROBABILITY 50
#define POPULATION_SIZE 1000


class Solution{
public:
    vector<int> cityes;

    Solution(int size){
       cityes = vector<int>(size,0);
       for(int i=0; i<size; i++){
           cityes[i] = i;
       }
    }

    void mutate(){
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
        int swap_size = rand()%(cityes.size()/2-1);
        int first_index = rand()%cityes.size();
        int second_index_max_distance = cityes.size()-2*swap_size;
        int second_index = (first_index+swap_size+rand()%second_index_max_distance)%cityes.size();

        for(int i=0; i<swap_size; i++){
            int x1 = (first_index+i)%cityes.size();
            int x2 = (second_index+i)%cityes.size();
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

    /*
    for(int i=0; i<size; i++){
        output << i << " ";
    }
    output <<"0 #" << endl;
    return 0;
    */

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

    Solution solution(size);

    while (true){

        for(auto e: solution.cityes){
            output << e << " ";
        }
        output << solution.cityes[0] << " #" << endl;

        solution = mutation_cycle(solution,distances);
        if(solution.cityes.empty()){
            break;
        }
    }


    output.close();
    input.close();

    return 0;
}
