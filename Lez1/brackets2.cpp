#include <istream>
#include <stdio.h>
#include <fstream>
#include <stack>

/// Funziona ma è troppo lento!!!

using namespace std;



bool is_correct(int * vec, int len, stack<int> &can_close){


    if (len == 0){
        return can_close.empty();
    }

    int bracket = *vec;

    // try open
    can_close.push(bracket);
    bool result = is_correct(vec+1,len-1,can_close);
    if(result){
        return true;
    }
    can_close.pop();

    // try close
    if(can_close.empty()){
        return false;
    }
    int last = can_close.top();

    // can't close
    if(last != bracket){
        return false;
    }

    can_close.pop();
    result = is_correct(vec+1,len-1,can_close);
    if(result){
        return true;
    }
    can_close.push(bracket);
    return false;
}


int test_one(FILE* file){
    int len, kind;

    fscanf(file,"%d %d\n",&len, &kind);

    stack<int> can_close = stack<int>();

    int* array = new int[len];

    for(int i=0; i<len; i++){
        fscanf(file, "%d", array+i);
    }

    bool res = is_correct(array,len,can_close);

    delete[] array;

    return res;
}


int main() {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    int num_lines;
    fscanf(input, "%d\n", &num_lines);

    for (int i = 0; i < num_lines; i++) {
        int val = 0;
        if (test_one(input)) {
            val = 1;
        }
        fprintf(output, "%d\n", val);
    }
}