#ifndef LOCAL
#include "got.h"
#endif

#define GENERATION_SIZE 150
#define MAX_N_ATTEMPTS 1000
#define N_MUTATIONS 1

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <algorithm>
#include <cassert>
#include <algorithm>
#include <random>
using namespace std;

template<typename T, typename E>
E & operator<<(E &os, vector<vector<T>> & v){
    for(size_t y=0; y<v[0].size(); y++){
        for(size_t x=0; x<v.size(); x++){
            os << v[x][y] << " ";

        }
        os << endl;
    }
    return os;
}

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

    bool operator< (Castle const& other) const {
        return size < other.size;
    }
};

class Point{
public:
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
};

class Solution {
public:
    static vector<Castle> castles;
    vector<vector<int>> tiles;
    static vector<vector<int>> castles_map;
    static vector<int> sizes;
    static vector<vector<bool>> mask;
    static vector<Castle> castle_to_mutate;
    static int size_x; // cols (M)
    static int size_y; // rows (N)

    Solution(ifstream& fs) {
        fs >> Solution::size_y >> Solution::size_x;

        tiles = vector<vector<int>>(size_x, vector<int>(size_y, 0));
        Solution::castles_map = vector<vector<int>>(size_x, vector<int>(size_y, 0));
        Solution::mask = vector<vector<bool>>(size_x, vector<bool>(size_y, false));
        Solution::castles = vector<Castle>();
        Solution::sizes = vector<int>();

        for (int y = 0; y < Solution::size_y; y++) {
            for (int x = 0; x < Solution::size_x; x++) {
                int v;
                fs >> v;
                if (v != 0) {
                    Solution::castles.emplace_back(x, y, v);
                    Solution::castles_map[x][y] = v;
                    tiles[x][y] = v;
                }
                if (find(sizes.begin(), sizes.end(), v) == sizes.end()) {
                    sizes.push_back(v);
                }
            }
        }

        sort(Solution::castles.begin(), Solution::castles.end());
    }

   void reset_mask(){
       for(int x=0; x<size_x; x++){
           for(int y=0; y<size_y; y++){
                mask[x][y] = false;
           }
       }
   };

   int get_tile(int x, int y) {
       if(
               x < 0 ||
               y < 0 ||
               x >= size_x ||
               y >= size_y
               ){
           return -1;
       }
       return tiles[x][y];
   }

   // return true if this component is ourside the considered area (and therefore the components adjacent to it are part of the border)
   bool find_border_r(int x, int y, int color, vector<Point>& border) {
       if(
               x < 0 ||
               y < 0 ||
               x >= Solution::size_x ||
               y >= Solution::size_y
               ){
           // the outside is by definiton a border
           return true;
       }
       if (Solution::mask[x][y]) {
           return false;
       }

       if (tiles[x][y] != color) {
           return true;
       }

       mask[x][y] = true;

       if (find_border_r(x - 1, y, color, border)) {
           border.emplace_back(x, y);
       }

       if (find_border_r(x + 1, y, color, border)) {
           border.emplace_back(x, y);
       }

       if (find_border_r(x, y + 1, color, border)) {
           border.emplace_back(x, y);
       }
       if (find_border_r(x, y - 1, color, border)) {
           border.emplace_back(x, y);
       }  

       return false;
   }

   vector<Point> find_border(int x, int y, int color) {
       reset_mask();
       vector<Point> border = vector<Point>();
       find_border_r(x, y, color, border);
       return border;
   }

   int mask_area_r(int x, int y, int color, vector<Point>* border, vector<vector<bool>>* out_area, bool set_out_area_to){
       if(
               x < 0 ||
               y < 0 ||
               x >= Solution::size_x ||
               y >= Solution::size_y
               ){
           return 0;
       }

      if(mask[x][y]){
          return 0;
      }
      mask[x][y] = true;

      if(tiles[x][y] != color) {
          if (border != nullptr) {
              border->emplace_back(x, y);
          }
          return 0;
      }
      else if (out_area != nullptr){
          (*out_area)[x][y] = set_out_area_to;
      }

      return 1  + mask_area_r(x-1,y,color, border, out_area, set_out_area_to)
                + mask_area_r(x+1,y,color, border, out_area, set_out_area_to)
                + mask_area_r(x,y-1,color, border, out_area, set_out_area_to)
                + mask_area_r(x,y+1,color, border, out_area, set_out_area_to);
   }

   int mask_area(int x, int y, vector<Point>* border, vector<vector<bool>>* out_area, bool set_out_area_to){
       reset_mask();
       int color = tiles[x][y];
       return mask_area_r(x,y,color, border, out_area, set_out_area_to);
   }

    float evaluate_single(Castle & c){
        int color = tiles[c.x][c.y];
        if(color != c.size){
            return 0;
        }
        int n = mask_area(c.x, c.y, nullptr, nullptr, false);

        if(n == c.size) {
            return 1.0;
        }

        castle_to_mutate.push_back(c);

        if(n < c.size){
            return (float) n / (float) c.size /  7;
        }
        return 0;
    }

   float evaluate(){
        castle_to_mutate = vector<Castle>();
        float score = 0;
        for(auto castle: castles){
           score += evaluate_single(castle);
        }
        return score;
   }

   // remove the numbers that are been let outside without a castle
   void clean() {
       reset_mask();
       auto to_keep = mask;

       // remove the numbers that are left, but don't have a castle
       for (auto& castle : castles) {
           // castle is already inclided... no need to include it again
           if (to_keep[castle.x][castle.y]) {
               continue;
           }
           mask_area(castle.x, castle.y, nullptr, &to_keep, true);
       }

       // remove the castle that has a size different from the obtimal one
       for (auto& castle : castles) {
           // castle is already inclided... no need to include it again
           if (to_keep[castle.x][castle.y] == false) {
               continue;
           }
           int area = mask_area(castle.x, castle.y, nullptr, nullptr, false);
           if (area != castle.size) {
                mask_area(castle.x, castle.y, nullptr, &to_keep, false);
           }
       }


       for (int x = 0; x < size_x; x++) {
           for (int y = 0; y < size_y; y++) {
               if (!to_keep[x][y]) {
                   tiles[x][y] = 0;
               }
           }
       }
   }
};




vector<Castle> Solution::castles = vector<Castle>();
vector<vector<int>> Solution::castles_map = vector<vector<int>>();
vector<int> Solution::sizes = vector<int>();
vector<vector<bool>> Solution::mask = vector<vector<bool>>();
vector<Castle> Solution::castle_to_mutate = vector<Castle>();

int Solution::size_x = 0; // cols (M)
int Solution::size_y = 0; // rows (N)

enum Result{
    SUCCESS,
    UNSUCCESS,
    NOTHING_TO_DO
};

// try shrinking the current solution without dividing it in two
Result try_shrink_castle(Solution & solution, Castle castle) {
    int area_before_shrink = solution.mask_area(castle.x, castle.y, nullptr, nullptr, false);

    // size is already perfect... no need to shrink!
    if (area_before_shrink <= castle.size) {
        return NOTHING_TO_DO;
    }

    auto points = solution.find_border(castle.x, castle.y, castle.size);

    shuffle(points.begin(),points.end(),std::default_random_engine(0));

    for (auto p : points) {
        // can't remove a castle
        if (Solution::castles_map[p.x][p.y] != 0) {
            continue;
        }
        solution.tiles[p.x][p.y] = 0;
        int area_after_shrink = solution.mask_area(castle.x, castle.y, nullptr, nullptr, false);

        // successfuly skrink without splitint the pieces
        if (area_after_shrink + 1 == area_before_shrink) {
            return SUCCESS;
        }

        // restore
        solution.tiles[p.x][p.y] = castle.size;
    }
    return UNSUCCESS;
}

Result try_expand_castle(Solution & solution, Castle castle, vector<vector<bool>>& protected_area, int max_recursion) {
    if (max_recursion == 0) {
        return UNSUCCESS;
    }
    
    if (solution.tiles[castle.x][castle.y] == 0) {
        solution.tiles[castle.x][castle.y] = castle.size;
        return SUCCESS;
    }

    // second choice are points that are not one castle of another size
    vector<Point>  choices = vector<Point>();

    vector<Point> border = vector<Point>();
    int area = solution.mask_area(castle.x, castle.y, &border, & protected_area, true);

    // castle is already big enougf, no need to expand it
    if (area >= castle.size) {
        return NOTHING_TO_DO;
    }

    for (auto p : border) {
        // i can't incorporate another castle that is not of my same color
        int castle_in_border = Solution::castles_map[p.x][p.y];
        if (castle_in_border != 0 && castle_in_border != castle.size) {
            continue;
        }

        // can't expand in a protected area (to avoid destroying our previous work to restore the current tile)
        if (protected_area[p.x][p.y]) {
            continue;
        }

        int current_border_color = solution.tiles[p.x][p.y];

        assert(current_border_color != castle.size);

        // prefer to expand where there the area already free
        if (current_border_color == 0) {
            solution.tiles[p.x][p.y] = castle.size;
            return SUCCESS;
        }


        Castle castle_to_restore = Castle(p.x, p.y, current_border_color);
        
        if (solution.get_tile(p.x + 1, p.y) == current_border_color) {
            castle_to_restore.x += 1;
        }
        else if (solution.get_tile(p.x - 1, p.y) == current_border_color) {
            castle_to_restore.x -= 1;
        }
        else if (solution.get_tile(p.x, p.y+1) == current_border_color) {
            castle_to_restore.y += 1;
        }
        else if (solution.get_tile(p.x, p.y-1) == current_border_color) {
            castle_to_restore.y -= 1;
        }
        else {
            // impossible in theory
            return UNSUCCESS;
            assert(false);
        }

        int size_castle_before_remove = solution.mask_area(castle_to_restore.x, castle_to_restore.y, nullptr, nullptr, false);

        solution.tiles[p.x][p.y] = castle.size;

        int size_castle_after_remove = solution.mask_area(castle_to_restore.x, castle_to_restore.y, nullptr, nullptr, false);

        // avoid splitting the area i take teh item from 
        if (size_castle_after_remove + 1 != size_castle_before_remove) {
            solution.tiles[p.x][p.y] = current_border_color;
            continue;
        }

        Result result = try_expand_castle(solution, castle_to_restore, protected_area, max_recursion - 1);

        if (result != UNSUCCESS) {
            return SUCCESS;
        }

        // if it didn't work i need to restore the solution before trying the next one
        
        solution.tiles[p.x][p.y] = current_border_color;
    }

    // if i am here it means that i couldn't increase the size of the current partition without destroying another one

    // unprotect the current area
    //solution.mask_area(castle.x, castle.y, nullptr, & protected_area, false);
    
    return UNSUCCESS;
}

Result try_expand_castle_non_recursive(Solution &solution, Castle castle){
    auto m = vector<vector<bool>>(Solution::size_x,vector<bool>(Solution::size_y, false));
    return try_expand_castle(solution,castle,m,3);
}

// expand the castle without warring about the destruction of other castles
Result expand_castle(Solution & solution, Castle castle) {
    if (solution.tiles[castle.x][castle.y] == 0) {
        solution.tiles[castle.x][castle.y] = castle.size;
        return SUCCESS;
    }

    // second choice are points that are not one castle of another size
    vector<Point>  first_choices = vector<Point>();
    vector<Point>  second_choices = vector<Point>();

    vector<Point> border = vector<Point>();
    int area = solution.mask_area(castle.x, castle.y, &border, nullptr, true);

    // castle is already big enougf, no need to expand it
    if (area >= castle.size) {
        return NOTHING_TO_DO;
    }

    for (auto p : border) {
        // i can't incorporate another castle that is not of my same color
        int castle_in_border = Solution::castles_map[p.x][p.y];
        if (castle_in_border != 0 && castle_in_border != castle.size) {
            continue;
        }

        int current_border_color = solution.tiles[p.x][p.y];

        assert(current_border_color != castle.size);

        if (current_border_color == 0) {
            first_choices.push_back(p);
        }else{
            second_choices.push_back(p);
        }
    }

    if(!first_choices.empty()){
        int n = rand()%first_choices.size();
        auto p = first_choices[n];
        solution.tiles[p.x][p.y] = castle.size;
        return SUCCESS;
    }
    if(!second_choices.empty()){
        int n = rand()%second_choices.size();
        auto p = second_choices[n];
        solution.tiles[p.x][p.y] = castle.size;
        return SUCCESS;
    }
    return UNSUCCESS;
}

// shrink the castle size, even if it need to be devided by two
Result shrink_castle(Solution & solution, Castle castle) {
    int area_before_shrink = solution.mask_area(castle.x, castle.y, nullptr, nullptr, false);

    // size is already perfect... no need to shrink!
    if (area_before_shrink <= castle.size) {
        return NOTHING_TO_DO;
    }

    auto points = solution.find_border(castle.x, castle.y, castle.size);

    if(points.size() <= 1){
        return UNSUCCESS;
    }

    auto p = points[rand()%points.size()];
    solution.tiles[p.x][p.y] = 0;
    return  SUCCESS;
}

void mutate(Solution& solution){

    if(Solution::castle_to_mutate.size() == 0){
        return;
    }

    auto castle = solution.castle_to_mutate[rand()%solution.castle_to_mutate.size()];


    int v = rand()%100;
    if(v < 30){
        try_expand_castle_non_recursive(solution,castle);
    }else if (v < 60){
        try_shrink_castle(solution,castle);
    }else if (v < 75){
        shrink_castle(solution,castle);
    }else{
        expand_castle(solution,castle);
    }
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
                mutate(to_mutate);
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
        auto to_print = current_best;
        //to_print.clean();
        of << to_print.tiles;
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