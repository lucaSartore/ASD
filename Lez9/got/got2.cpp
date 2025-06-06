#ifndef LOCAL
#include "got.h"
#endif

#define GENERATION_SIZE 100
#define MAX_N_ATTEMPTS 70
#define N_MUTATIONS 5

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <algorithm>
#include <cassert>
#include <algorithm>
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

int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    Solution s = Solution(input);
    for (auto& castle : Solution::castles) {
        auto protected_area = Solution::mask;
        do {
            s.reset_mask();
            protected_area = Solution::mask;
        } while (try_expand_castle(s, castle, protected_area, 3) == SUCCESS);
        while(try_shrink_castle(s, castle) == SUCCESS);
        s.clean();
        output << s.tiles << "***" << endl;
    }
    

    output.close();
    input.close();
    return 0;
}