//
// Created by lucas on 03/10/2023.
//


#include <stdio.h>
using namespace std;

int main(){
    auto input = fopen("input.txt","r");
    auto output = fopen("output.txt","w");

    int n1,n2;

    fscanf(input,"%d %d", &n1, &n2);

    fprintf(output,"%d",n1+n2);

    fclose(input);
    fclose(output);
}
