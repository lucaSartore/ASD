//
// Created by lucas on 23/03/2024.
//
#include <iostream>
#include <fstream>
#include <vector>
using  namespace  std;

int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    string w1, w2;

    input >> w1 >> w2;

    auto v1 = vector<int>(w1.length(),0);
    auto v2 = vector<int>(w1.length(),0);

    auto memory = &v1;
    auto memory_pre = &v2;

    for( int i2=0; i2<w2.length(); i2++){
        for(int i1=0; i1<w1.length(); i1++){
           int value_left = 0;
           int value_top = memory_pre->operator[](i1);
           int value_top_left = 0;
           if(i1 != 0){
               value_left = memory->operator[](i1-1);;
               value_top_left = memory_pre->operator[](i1-1);
           }
           if(w1[i1] == w2[i2]){
               value_top_left++;
           }
           memory->operator[](i1) = max(max(value_top_left,value_left),value_top);
        }
        auto tmp = memory_pre;
        memory_pre = memory;
        memory = tmp;
    }

    output << *memory_pre->rbegin();

    input.close();
    output.close();
    return 0;
}
