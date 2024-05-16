#ifndef LOCAL
#include "got.h"
#endif

#define GENERATION_SIZE 200
#define MAX_N_ATTEMPTS 70
#define N_MUTATIONS 5
#define MAX_TOTAL_REATTEMPT 30000000000

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <bits/stdc++.h>
using namespace std;

class Castle{
public:
    int size;
    int x;
    int y;

    Castle(int _x, int _y, int _size){
        x = _x;
        y = _y;
        size = _size;
    }
};

class Point{
public:
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
};

class Solution{
public:
   vector<Castle> castles;
   vector<vector<int>> tiles;
   vector<vector<bool>> mask;
   vector<int> sizes;
   int size_x; // cols (M)
   int size_y; // rows (N)

   Solution(ifstream & fs){
        fs >> size_y >> size_x;

        tiles = vector<vector<int>>(size_x, vector<int>(size_y,0));
        mask = vector<vector<bool>>(size_x, vector<bool>(size_y,false));
        castles = vector<Castle>();
        sizes = vector<int>();

       for(int y=0; y<size_y; y++){
           for(int x=0; x<size_x; x++){
                int v;
                fs >> v;
                if(v != 0){
                    castles.emplace_back(x,y,v);
                    tiles[x][y] = v;
                }
                if(find(sizes.begin(),sizes.end(),v) == sizes.end()){
                    sizes.push_back(v);
                }
            }
        }
   }

   void reset_mask(){
       for(int x=0; x<size_x; x++){
           for(int y=0; y<size_y; y++){
                mask[x][y] = false;
           }
       }
   };

   int mask_area_r(int x, int y, int color){
       if(
               x < 0 ||
               y < 0 ||
               x >= size_x ||
               y >= size_y
               ){
           return 0;
       }

      if(tiles[x][y] != color || mask[x][y]){
          return 0;
      }

      mask[x][y] = true;
      return 1  + mask_area_r(x-1,y,color)
                + mask_area_r(x+1,y,color)
                + mask_area_r(x,y-1,color)
                + mask_area_r(x,y+1,color);
   }

   int mask_area(int x, int y){
       reset_mask();
       int color = tiles[x][y];
       return mask_area_r(x,y,color);
   }

   float evaluate_single(Castle & c){
        int color = tiles[c.x][c.y];
        if(color != c.size){
            return 0;
        }
        int n = mask_area(c.x, c.y);
        if(n == c.size) {
            return 1.0;
        }
        if(n < c.size){
            return (float) n / (float) c.size /  2.0;
        }
       return 0;
   }

   bool is_masked(int x, int y){
       if(
               x < 0 ||
               y < 0 ||
               x >= size_x ||
               y >= size_y
               ){
           return false;
       }
       return mask[x][y];
   }

   vector<Point> get_adj_tiles(Castle & c){
       // no score means the castle is not in the map, therefore the only adjacent tile is the castle itself
       if(evaluate_single(c) == 0){
           return vector<Point>{Point(c.x,c.y)};
       }
       vector<Point> points = vector<Point>();
       // now the mask is filled
       for(int x=0; x<size_x; x++){
           for(int y=0; y<size_y; y++){
               if(!mask[x][y] && (
                       is_masked(x-1,y) ||
                       is_masked(x+1,y) ||
                       is_masked(x,y-1) ||
                       is_masked(x,y+1)
                       )){
                   points.emplace_back(x,y);
               }
           }
       }
       return points;
   }

   void mutate(){

       add_tile_mutation();
       return;
       switch (rand()%3) {
           case 0:
               add_tile_mutation();
               break;
           case 1:
               swap_tile_mutation();
               break;
           case 2:
               remove_tile_mutation();
               break;
           default:
               exit(1);
       }
   }



   void add_tile_mutation(){
       auto castle_to_mutate = castles[rand()%castles.size()];
       auto points = get_adj_tiles(castle_to_mutate);
       auto point = points[rand()%points.size()];
       tiles[point.x][point.y] = castle_to_mutate.size;
   }

   void swap_tile_mutation(){
       auto castle_to_mutate = castles[rand()%castles.size()];
       auto points = get_adj_tiles(castle_to_mutate);
       auto point1 = points[rand()%points.size()];
       auto point2 = points[rand()%points.size()];
       tiles[point1.x][point2.y] = castle_to_mutate.size;
       tiles[point2.x][point2.y] = 0;
   }

   void remove_tile_mutation(){
       auto castle_to_mutate = castles[rand()%castles.size()];
       auto points = get_adj_tiles(castle_to_mutate);
       auto point = points[rand()%points.size()];
       tiles[point.x][point.y] = 0;
   }

   float evaluate(){
       float score = 0;
       for(auto & c: castles){
           score += evaluate_single(c);
       }
       return score;
   }

   void print_solution(ofstream &of){
       for(int y=0; y<size_y; y++){
           for(int x=0; x<size_x; x++){
               of << tiles[x][y] << " ";
           }
           of << endl;
       }
   }

   void mask_map(){
       for(int y=0; y<size_y; y++){
           for(int x=0; x<size_x; x++){
               if(mask[x][y]){
                   tiles[x][y] = 0;
               }
           }
       }
   }

   void clean(){

       for(auto& castle: castles){
           if(tiles[castle.x][castle.y] != castle.size){
               continue;
           }
           int area = mask_area(castle.x,castle.y);
           if(area != castle.size){
              mask_map();
           }
       }
   }
};


ostream& operator<<(ostream &os, Solution & s){
    for(int y=0; y<s.size_y; y++){
        for(int x=0; x<s.size_x; x++){
            os << s.tiles[x][y] << " ";
        }
        os << endl;
    }
    return os;
}

void print_solution(Solution solution, ofstream& of){
    solution.clean();
    solution.print_solution(of);
}

int find_solution(ofstream& of, Solution current_solution, float absolute_best){
    Solution current_best = current_solution;
    float max_score = 0;
    int n_attempts = 0;
    while (n_attempts<MAX_N_ATTEMPTS){
        bool has_improve = false;
        //cout << "Original:" << endl << current_best << ends;
        for(int i=0; i<GENERATION_SIZE; i++){
            Solution to_mutate = current_best;
            for(int j=0; j<N_MUTATIONS; j++){
                to_mutate.mutate();
            }
            //cout << "Mutated:" << endl << to_mutate<< endl;
            float score = to_mutate.evaluate();
            //cout << "Score: " << score << endl;
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
        print_solution(current_best,of);
        of << "***" << endl;
    }
    return absolute_best;
}


int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    Solution s = Solution(input);

    find_solution(output,s,0);

    output.close();
    input.close();
    return 0;
}