#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stack>
#include <stdio.h>
#include <list>

//#define DEBUG
//#define OUTPUT_FILE

/// Funziona ma è troppo lento!!!

using namespace std;


list<int> merge_lists(list<int> &&left, list<int> &&right){

    while (true){
        if(left.empty() || right.empty()){
            break;
        }
        if(left.back() == right.front()){
            left.pop_back();
            right.pop_front();
        } else {
            break;
        }
    }

    left.insert(left.end(),right.begin(),right.end());

    return left;
}

void put_tab(int n){
    for(int _=0; _<n; _++){
        cout << "\t";
    }
}

list<int> simplify(list<int> &&brackets, int tab_size = 1){
    if(brackets.size() <= 1){
        return brackets;
    }

    int split_at = brackets.size()/2;


#ifdef DEBUG
    put_tab(tab_size);cout << "===========================================================" << endl;

    put_tab(tab_size);cout << "input: ";
    for (const auto &s : brackets )std::cout << s << ' ';
    std::cout << std::endl;
#endif


    list<int> left = std::move(brackets);
    list<int> right = list<int>();

    right.splice(right.begin(),left,std::next(left.begin(),split_at), left.end());

#ifdef DEBUG
    put_tab(tab_size);cout << "left: ";
    for (const auto &s : left ) std::cout << s << ' ';
    std::cout << std::endl;

    put_tab(tab_size);cout << "right: ";
    for (const auto &s : right ) std::cout << s << ' ';
    std::cout << std::endl;
#endif

    if(left.size() > 1){
        left = simplify(std::move(left),tab_size+1);
    }
    if(right.size() > 1){
        right = simplify(std::move(right),tab_size+1);
    }

    list<int> to_return = merge_lists(std::move(left), std::move(right));

#ifdef DEBUG
    put_tab(tab_size);cout << "merged: ";
    for (const auto &s : to_return )cout << s;
    std::cout << std::endl;
#endif

    return to_return;
}

// is correct only if it gets simplified to 100%
bool is_correct(list<int> &&brackets){
    return simplify(std::move(brackets)).empty();
}


int test_one(FILE* file){
    int len, kind;

    fscanf(file,"%d %d\n",&len, &kind);


    list<int> brackets = list<int>();

    for(int i=0; i<len; i++){
        int val;
        fscanf(file, "%d", &val);
        brackets.push_back(val);
    }


    return is_correct(move(brackets));
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
#ifndef OUTPUT_FILE
        if(output){
            printf("The sequence %d was valid!\n",i);
        }else{
            printf("The sequence %d was invalid!\n",i);
        }
#else
        fprintf(output, "%d\n", val);
#endif
    }
}