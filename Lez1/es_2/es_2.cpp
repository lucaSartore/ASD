//
// Created by lucas on 03/10/2023.
//

#include "es_2.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
using namespace std;

// prendi 2 numeri da un file e sommali su output file
void es_2(){
    auto input = fopen("../es_2/input.txt","r");
    auto output = fopen("../es_2/output.txt","w");

    int l;
    fscanf_s(input,"%d\n", &l);


    int integral = 0;

    int min_integral = 0;

    int max = 0;

    for (int i=0; i<l; i++){
        int n;
        fscanf_s(input,"%d", &n);

        integral += n;

        if(n>0){
            int potential_max = integral - min_integral;

            if(potential_max > max){
                max = potential_max;
            }
        }

        if(integral < min_integral){
            min_integral = integral;
        }
    }

    fprintf(output,"%d",max);

    fclose(input);
    fclose(output);
}
