#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stack>
#include <stdio.h>
#include <list>
#include <chrono>

//#define DEBUG
//#define OUTPUT_FILE

/// Funziona ma è troppo lento!!!

using namespace std;

chrono::duration<long long, ratio<1,1000000>> time_spent_merge_function = chrono::duration<long long, ratio<1,1000000>>(0);
chrono::duration<long long, ratio<1,1000000>> time_spent_split = chrono::duration<long long, ratio<1,1000000>>(0);
chrono::duration<long long, ratio<1,1000000>> time_spent_merging = chrono::duration<long long, ratio<1,1000000>>(0);


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

    //auto start = chrono::high_resolution_clock::now();
    left.insert(left.end(),right.begin(),right.end());
    //auto stop = chrono::high_resolution_clock::now();

    //time_spent_merging += chrono::duration_cast<chrono::microseconds>(stop - start);

    return left;
}

void put_tab(int n){
    for(int _=0; _<n; _++){
        cout << "\t";
    }
}

list<int> simplify(list<int> &&brackets, int tab_size = 1){

    //auto start = chrono::high_resolution_clock::now();
    int split_at = brackets.size()/2;

#ifdef DEBUG
    put_tab(tab_size);cout << "===========================================================" << endl;

    put_tab(tab_size);cout << "input: ";
    for (const auto &s : brackets )std::cout << s << ' ';
    std::cout << std::endl;
#endif

    list<int> left = std::move(brackets);
    list<int> right = list<int>();

    right.splice(right.begin(),std::move(left),std::next(left.begin(),split_at), left.end());
    //auto stop = chrono::high_resolution_clock::now();

    //time_spent_split += chrono::duration_cast<chrono::microseconds>(stop - start);

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

    //start = chrono::high_resolution_clock::now();
    list<int> to_return = merge_lists(std::move(left), std::move(right));
    //stop = chrono::high_resolution_clock::now();

    //time_spent_merge_function += chrono::duration_cast<chrono::microseconds>(stop - start);

#ifdef DEBUG
    put_tab(tab_size);cout << "merged: ";
    for (const auto &s : to_return )cout << s;
    std::cout << std::endl;
#endif

    return to_return;
}

// is correct only if it gets simplified to 100%
bool is_correct(list<int> &&brackets){

    auto start = chrono::high_resolution_clock::now();
    bool result = simplify(std::move(brackets)).empty();
    auto stop = chrono::high_resolution_clock::now();


    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Execution time C++: "<< duration.count() << " us" << endl;
    cout << "\t Split is: "<< (float)time_spent_split.count()/duration.count() * 100 << " % of the execution or "<<  time_spent_split.count() << " us"<< endl;
    cout << "\t Merge is: " << (float)time_spent_merging.count() / duration.count()  * 100<< " % of the execution or " << time_spent_merging.count() << " us" << endl;
    cout << "\t Merge function is: " << (float)time_spent_merge_function.count() / duration.count() << " % of the execution or " << time_spent_merge_function.count() << " us" << endl;
    return result;
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
        if(val){
            printf("The sequence %d was valid!\n",i);
        }else{
            printf("The sequence %d was invalid!\n",i);
        }
#else
        fprintf(output, "%d\n", val);
#endif
    }
}