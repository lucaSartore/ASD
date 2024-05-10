//
// Created by lucas on 23/03/2024.
//
//#include "tsp.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>

using namespace std;

typedef vector<vector<int>> graph;

class CityPair{
public:
    int next_city;
    int prev_city;

    void revert(){
        int tmp = prev_city;
        prev_city = next_city;
        next_city = tmp;
    }
};

class Solution{
public:
    vector<CityPair> path;

    Solution(int size){
        path = vector<CityPair>(size,CityPair());

        for(int i=0; i<size; i++){
            path[i].next_city = (i + 1) % size;;
            path[i].prev_city = (i - 1 + size) % size;
        }
    }

    bool mutate(graph & g){
        int max_reduction = 0;
        int best_i;
        int best_j;

        for(int i=0; i<path.size(); i++){
            for(int j = i+2; j<path.size(); j++){
                auto pj = path[j];
                auto pi = path[i];
                int current_cost = g[j][pj.next_city] + g[i][pi.next_city];

                int new_score = g[i][j] + g[pi.next_city][pj.next_city];

                int reduction = current_cost - new_score;

                if(reduction > max_reduction){
                    max_reduction = reduction;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if(max_reduction == 0){
            return false;
        }

        // re make order;

        int from_i = best_i;
        int to_i = path[best_i].next_city;

        int from_j = best_j;
        int to_j = path[best_j].next_city;


        // fix to_i
        path[to_i].revert();
        path[to_i].next_city = to_j;

        // fix to_j
        path[to_j].prev_city = to_i;

        // fix from_i
        path[from_i].next_city = from_j;

        //fix from_j
        path[from_j].revert();
        path[from_j].prev_city = from_i;

        int current_city = path[from_j].next_city;
        int destination_city = to_i;


        while (current_city != destination_city){
            path[current_city].revert();
            current_city = path[current_city].next_city;
        }




        return true;
    }

    int calculate_distance(graph & g){
        int c = 0;
        for(int i=0; i<path.size(); i++){
           c += g[i][path[i].next_city];
        }
        return c;
    }
};



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

    Solution solution(size);

    while (solution.mutate(distances)){

        cout << solution.calculate_distance(distances) << endl;

        for(auto e: solution.path){
            output << e.next_city << " ";
        }
        output << solution.path[0].next_city << " #" << endl;
    }

    for(auto e: solution.path){
        output << e.next_city << " ";
    }
    output << solution.path[0].next_city << " #" << endl;

    output.close();
    input.close();

    return 0;
}
