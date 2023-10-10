#include <stdio.h>


int main(){
    auto input = fopen("input.txt","r");
    auto output = fopen("output.txt","w");

    int l;
    fscanf(input,"%d\n", &l);


    int integral = 0;
    int min_integral = 0;
    int max = 0;

    for (int i=0; i<l; i++){
        int n;
        fscanf(input,"%d", &n);

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
