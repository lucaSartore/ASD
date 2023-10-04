//
// Created by lucas on 03/10/2023.
//

#include "es_3.h"
#include "../es_2/es_2.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <assert.h>
using namespace std;


class MyMat{
public:

    int size_x;
    int size_y;

    vector<vector<int>> mat_og;
    // first is x second is y
    vector<vector<int>> mat_integral;

    static int get_or_zero(vector<vector<int>>& mat, int x, int y){
        if (x == -1 || y == -1){
            return 0;
        }
        return mat[x][y];
    }

    /// initialize the matrix form an input file.
    /// create the rwo support matrix and make them empty
    MyMat(char* path){

        auto f = fopen(path, "r");

        fscanf_s(f,"%d %d", &size_y, &size_x);

        for(auto mat: {&mat_og, &mat_integral}){
            *mat = vector<vector<int>>();
            mat->reserve(size_x);
            for(int x=0; x<size_x; x++){
                auto new_mat = vector<int>();
                new_mat.reserve(size_y);
                for(int y=0; y<size_y; y++){
                    new_mat.push_back(0);
                }
                mat->push_back(std::move(new_mat));
            }
        }


        for(int y=0; y<size_y; y++){
            for(int x=0; x<size_x; x++){
                fscanf_s(f,"%d", &(mat_og[x][y]));
            }
        }

    };

    /// initialize the matrix containing the 2d integral
    void init_mat_integral(){
        for(int y=0; y<size_y; y++){
            for(int x=0; x<size_x; x++){
                mat_integral[x][y] = get_or_zero(mat_integral,x-1,y) + get_or_zero(mat_integral,x,y-1) - get_or_zero(mat_integral,x-1,y-1) + mat_og[x][y];
            }
        }
    }

    int sum_on(int x1,int y1, int x2, int y2){
        return get_or_zero(mat_integral,x2,y2) + get_or_zero(mat_integral,x1-1,y1-1) - get_or_zero(mat_integral,x2,y1-1) - get_or_zero(mat_integral,x1-1,y2);
    }


    /// find the maximum value
    int find_max(){

        if (size_x == 0 || size_y == 0){
            return  0;
        }

        int small_size;
        int big_size;
        bool x_is_bigger;

        if (size_x > size_y){
            x_is_bigger = true;
            big_size = size_x;
            small_size = size_y;
        }else{
            x_is_bigger = false;
            big_size = size_y;
            small_size = size_x;
        }

        auto get_sub_sum = [this, x_is_bigger](int big, int small_1, int small_2) {
            if(x_is_bigger){
                return this->sum_on(big,small_1,big,small_2);
            }else{
                return this->sum_on(small_1,big,small_2,big);
            }
        };

        vector<int> partial_sum = vector<int>();
        partial_sum.reserve(big_size);
        for(int i=0; i<big_size; i++){partial_sum.push_back(0);}

        int max = get_sub_sum(0,0,0);

        for(int s2=0; s2 < small_size; s2++){
            for(int s1=0; s1 <= s2; s1++){
                for(int i=0; i<big_size; i++){
                    partial_sum[i] = get_sub_sum(i,s1,s2);
                }

                int max_this_row = max_subarray(partial_sum);

                if(max_this_row >  max){
                    max = max_this_row;
                }
            }
        }
        return max;
    }
};


ostream& operator<<(ostream& os,const vector<vector<int>> mat){
    int size_x = mat.size();
    if(size_x == 0)return os;

    int size_y = mat[0].size();
    if(size_y == 0)return os;

    for(int y=0; y<size_y; y++){
        for(int x=0; x<size_x; x++){
            os << mat[x][y] << "  ";
        }
        os << endl;
    }
    return os;
}

ostream& operator<<(ostream& os,const MyMat& myMat){

    os << "Original Mat: " << endl << myMat.mat_og  << endl << "---------------------------------------------" << endl;
    os << "Integral Mat: " << endl << myMat.mat_integral << endl << "---------------------------------------------" << endl;

    return os;
}

MyMat get_mat_from_file(char* file){
    MyMat myMat = MyMat(file);
    myMat.init_mat_integral();
    return myMat;
}

int get_output_from_file(char* file){
    MyMat myMat = get_mat_from_file(file);
    //cout << myMat << endl;
    return myMat.find_max();
}

// prendi 2 numeri da un file e sommali su output file
void es_3(){

    int v = get_output_from_file("../es_3/input_6.txt");
    cout << v <<"=6?" << endl;
    assert(v == 6);

    v = get_output_from_file("../es_3/input_18.txt");
    cout << v <<"=18?" << endl;
    assert(v == 18);

    v = get_output_from_file("../es_3/input_15.txt");
    cout << v <<"=15?" << endl;
    assert(v == 15);


}
