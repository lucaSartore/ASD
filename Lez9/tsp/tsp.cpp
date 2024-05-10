//
// Created by lucas on 23/03/2024.
//
//#include "tsp.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
using namespace std;


void find_path(int& current_min, vector<vector<int>>& distances, vector<bool>& visited, int current_distance, int current_node, int size, vector<int> current_path, int count, ofstream& fs){

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
        for(auto e: current_path){
            fs << e << " ";
        }
        fs << "0 #" << endl;
        return;
    }

    visited[current_node] = true;

    for(int i=0; i<size; i++){
       if(visited[i]){
           continue;
       }
       find_path(current_min, distances,visited, current_distance + distances[i][current_node], i, size,current_path,count+1, fs);
   }

    visited[current_node] = false;
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

   int min_distance = INT_MAX;

   vector<bool> visited = vector<bool>(size, false);
   vector<int> path = vector<int>(size,0);


    find_path(min_distance,distances,visited,0,0,size,path,0, output);

    cout << min_distance;

    output.close();
    input.close();

    return 0;
}