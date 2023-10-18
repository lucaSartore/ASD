#include <iostream>
#include <fstream>
#include <cstring>
#include <execution>
using namespace std;

void merge_sort(int* array_merge, int* support_array, int len);

int main(){

    //============== read the files ===============
    ifstream input("input.txt");

    int len;
    input >> len;

    int* array = new int[len];
    int* support_array = new int[len];

    for(int i=0; i<len; i++) input >> array[i];

    merge_sort(array,support_array,len);

    //for(int i=0; i<len; i++) cerr << array[i] << " ";

    ofstream output("output.txt");

    for(int i=0; i<len-1; i++) output << array[i] << " "; output << array[len-1];

    input.close();
    output.close();

    delete[] array;
    delete[] support_array;
}


void merge_r(int* v1, int* v2, int len_v1, int len_v2, int* support_array){
    if(len_v1 == 0 && len_v2 == 0){
        return;
    }
    if (len_v1 == 0){
        memcpy((void*) support_array, (void*) v2, sizeof(int) * (len_v2));
        return;
    }
    if (len_v2 == 0){
        memcpy((void*) support_array, (void*) v1, sizeof(int) * (len_v1));
        return;
    }
    if (*v1 < *v2){
        *support_array = *v1;
        merge_r(v1+1, v2,len_v1-1, len_v2, support_array + 1);
    }else{
        *support_array = *v2;
        merge_r(v1,v2+1, len_v1,len_v2-1, support_array + 1);
    }
}

void merge(int* v1, int* v2, int len_v1, int len_v2, int* support_array){
    merge_r(v1, v2, len_v1, len_v2, support_array);
    memcpy((void*) v1, (void*) support_array, sizeof(int) * (len_v1 + len_v2));
}

void merge_sort(int* array_merge, int* support_array, int len){
    if (len <= 1){
        return;
    }
    int split_at = len/2;

    int* v1 = array_merge;
    int* v2 = array_merge+split_at;

    int len_v1 = split_at;
    int len_v2 = len-split_at;

    //cout << len_v1 << " " << len_v2 << endl;

    merge_sort(v1,support_array,len_v1);
    merge_sort(v2,support_array,len_v2);

    merge(v1,v2,len_v1,len_v2,support_array);
}
