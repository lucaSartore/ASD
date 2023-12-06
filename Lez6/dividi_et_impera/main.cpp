#include <assert.h>
#include <iostream>
using namespace std;

void test_punto_fisso();
bool punto_fisso(int* vec, int start, int end);

void test_min_unimodulare();
int min_unimodulare(int* vec, int start, int end);

int sammarcanda(int* vec, int len);

int main(){
    test_punto_fisso();
    test_min_unimodulare();
}


void test_punto_fisso(){

    int v1[] = {-1,1,4,7,10,11};
    assert(punto_fisso(v1,0,5));
    cout << "OK1" << endl;

    int v2[] = {-1,3,4,7,10,11};
    assert(!punto_fisso(v2,0,5));
    cout << "OK2" << endl;

    int v3[] = {-1,0,1,2,3,4,6,7,8,9,10};
    assert(punto_fisso(v3,0,10));
    cout << "OK3" << endl;
}

bool punto_fisso(int* vec, int start, int end){
    if(start == end){
        return vec[start] == start;
    }
    if(start+1 == end){
        return vec[start] == start || vec[end] == end;
    }

    int mid_index = (start+end)/2;
    int mid_value = vec[mid_index];

    if(mid_value < mid_index){
        return punto_fisso(vec,mid_index,end);
    }else{
        return punto_fisso(vec,start,mid_index);
    }
}

void test_min_unimodulare(){
    int v1[] = {10,9,8,7,8,9,10};
    assert(min_unimodulare(v1,0,6) == 7);
    cout << "OK1" << endl;

    int v2[] = {10,9,8,7,6,5};
    assert(min_unimodulare(v2,0,5) == 5);
    cout << "OK1" << endl;

    int v3[] = {1,2,3,4,5,6,7,8,9};
    assert(min_unimodulare(v3,0,9) == 1);
    cout << "OK1" << endl;

    int v4[] = {9,8,7,6,5,4,3,2,1,2,3,4,5};
    assert(min_unimodulare(v4,0,12) == 1);
    cout << "OK1" << endl;
}

int min_unimodulare(int* vec, int start, int end){
    if(start == end){
        return vec[start];
    }
    if(start+1 == end){
        return min(vec[start],vec[end]);
    }

    int middle_low = (start+end)/2;
    int middle_high = middle_low+1;

    if(vec[middle_low] < vec[middle_high]){
        // going up
        return min_unimodulare(vec,start,middle_low);
    }else{
        // going down
        return min_unimodulare(vec,middle_high,end);
    }
}