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
using namespace std;

template<typename T>
ostream& operator<<(ostream &os, vector<vector<T>> & v){
    for(int y=0; y<v[0].size; y++){
        for(int x=0; x<v.size; x++){
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
};

class Point{
public:
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
};

class Solution {
public:
    vector<Castle> castles;
    vector<vector<int>> tiles;
    vector<vector<bool>> mask;
    vector<int> sizes;
    int size_x; // cols (M)
    int size_y; // rows (N)

    Solution(ifstream& fs) {
        fs >> size_y >> size_x;

        tiles = vector<vector<int>>(size_x, vector<int>(size_y, 0));
        mask = vector<vector<bool>>(size_x, vector<bool>(size_y, false));
        castles = vector<Castle>();
        sizes = vector<int>();

        for (int y = 0; y < size_y; y++) {
            for (int x = 0; x < size_x; x++) {
                int v;
                fs >> v;
                if (v != 0) {
                    castles.emplace_back(x, y, v);
                    tiles[x][y] = v;
                }
                if (find(sizes.begin(), sizes.end(), v) == sizes.end()) {
                    sizes.push_back(v);
                }
            }
        }
    }

};



int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    Solution s = Solution(input);


    output.close();
    input.close();
    return 0;
}