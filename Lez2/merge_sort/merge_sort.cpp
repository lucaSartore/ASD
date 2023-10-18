#include <iostream>
#include <fstream>
using namespace std;

void merge_sort(int* array_merge, int* support_array, int len);

int main(){

    //============== read the files ===============
    fstream input;
    input.open("input.txt");

    int len;
    input >> len;

    int* array = new int[len];
    int* support_array = new int[len];

    for(int i=0; i<len; i++) input >> array[i];

    merge_sort(array,support_array,len);

    for(int i=0; i<len; i++) cout << array[i] << " ";

    fstream output;
    output.open("output.txt", fstream::out);

    for(int i=0; i<len; i++) output << array[i] << " ";

    input.close();
    output.close();

    delete[] array;
    delete[] support_array;
}


void merge_r(int* v1, int* v2, int len_v1, int len_v2, int* destination){
    if(len_v1 == 0 && len_v2 == 0){
        return;
    }
    if (len_v1 == 0){
        memcpy((void*) v2,(void*) destination, sizeof(int)*(len_v2));
        return;
    }
    if (len_v2 == 0){
        memcpy((void*) v1,(void*) destination, sizeof(int)*(len_v1));
        return;
    }
    if (*v1 < *v2){
        *destination = *v1;
        merge_r(v1+1,v2,len_v1-1,len_v2,destination+1);
    }else{
        *destination = *v2;
        merge_r(v1,v2+1,len_v1,len_v2-1,destination+1);
    }
}

void merge(int* v1, int* v2, int len_v1, int len_v2, int* destination){
    merge_r(v1,v2,len_v1,len_v2,destination);
    memcpy((void*) v1,(void*) destination, sizeof(int)*(len_v1+len_v2));
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

    cout << len_v1 << " " << len_v2 << endl;

    merge_sort(v1,support_array,len_v1);
    merge_sort(v2,support_array,len_v2);

    merge(v1,v2,len_v1,len_v2,support_array);
}
