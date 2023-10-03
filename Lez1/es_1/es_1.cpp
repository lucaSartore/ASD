//
// Created by lucas on 03/10/2023.
//

#include "es_1.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;

// prendi 2 numeri da un file e sommali su output file
void es_1(){
    auto input = fopen("../input.txt","r");
    auto output = fopen("../output.txt","w");

    int n1,n2;

    fscanf_s(input,"%d %d", &n1, &n2);

    fprintf(output,"%d",n1+n2);

    fclose(input);
    fclose(output);
}
