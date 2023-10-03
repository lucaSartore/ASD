//
// Created by lucas on 03/10/2023.
//

#include "es_3.h"
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
    vector<vector<int>> mat_integral;
    vector<vector<int>> mat_neg_integral;
    vector<vector<int>> mat_neg_integral_partial_max;

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

        for(auto mat: {&mat_og, &mat_integral, &mat_neg_integral_partial_max, &mat_neg_integral}){
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

    /// initialize the matrix containing the 2d integral
    void init_mat_neg_integral(){
        for(int y=0; y<size_y; y++){
            for(int x=0; x<size_x; x++){
                mat_neg_integral[x][y] = get_or_zero(mat_integral,x-1,y-1) - get_or_zero(mat_integral,x,y-1) - get_or_zero(mat_integral,x-1,y);
            }
        }
    }

    /// initialize the matrix containing minimum integral behind
    /// can be called only after mat neg integral is initialized
    void init_mat_neg_integral_partial_max(){
        for(int y=0; y<size_y; y++){
            for(int x=0; x<size_x; x++){
                mat_neg_integral_partial_max[x][y] = max(get_or_zero(mat_neg_integral_partial_max, x - 1, y), get_or_zero(mat_neg_integral_partial_max, x, y - 1));
                mat_neg_integral_partial_max[x][y] = max(mat_neg_integral_partial_max[x][y], mat_neg_integral[x][y]);
            }
        }
    }

    /// find the maximum value
    /// can be called only after init_mat_neg_integral_partial_max is initialized
    int find_max(){
        int max_v = 0;
        for(int y=0; y<size_y; y++){
            for(int x=0; x<size_x; x++){
                max_v = max(max_v, mat_integral[x][y] + mat_neg_integral_partial_max[x][y]);
            }
        }
        return  max_v;
    }
};

vector<vector<int>> mat_min;

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
    os << "Neg Integral Mat: " << endl << myMat.mat_neg_integral << endl << "---------------------------------------------" << endl;
    os << "Max Mat: " << endl << myMat.mat_neg_integral_partial_max;

    return os;
}

MyMat get_mat_from_file(char* file){
    MyMat myMat = MyMat(file);
    myMat.init_mat_integral();
    myMat.init_mat_neg_integral();
    myMat.init_mat_neg_integral_partial_max();
    return myMat;
}

int get_output_from_file(char* file){
    MyMat myMat = get_mat_from_file(file);
    cout << myMat << endl;
    return myMat.find_max();
}

// prendi 2 numeri da un file e sommali su output file
void es_3(){
    /*
    int v = get_output_from_file("../es_3/input_6.txt");
    cout << v <<"=6?" << endl;
    //assert(v == 6);

    v = get_output_from_file("../es_3/input_18.txt");
    cout << v <<"=18?" << endl;
    //assert(v == 18);

    v = get_output_from_file("../es_3/input_15.txt");
    cout << v <<"=15?" << endl;
    //assert(v == 15);*/


    MyMat myMat = get_mat_from_file("../es_3/input_15.txt");

    cout << myMat << endl;

    cout << myMat.sum_on(1,1,2,2) << endl;

    cout << myMat.sum_on(1,2,2,2) << endl;
}
