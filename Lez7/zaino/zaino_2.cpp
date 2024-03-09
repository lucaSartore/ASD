#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <tuple>
#include <climits>
#include <iostream>

using namespace std;



void printv(vector<int>& v){
    cout << "[" << endl;
    int c = 0;
    for(auto node: v){
        cout << "\t" << c <<": " << node << "\n";
        c++;
    }
    cout << "]\n";

}


int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    int capacity, number_of_objects;

    input >> capacity >> number_of_objects;

    vector<int> costs = vector<int>();
    vector<int> weights = vector<int>();

    costs.reserve(number_of_objects);
    weights.reserve(number_of_objects);

    for(int i=0; i<number_of_objects; i++){
        int weight,cost;
        input >> weight >> cost;
        weights.push_back(weight);
        costs.push_back(cost);
    }


    vector<int> vector_1 = vector<int>(capacity+1,0);
    vector<int> vector_2 = vector<int>(capacity+1,0);

    vector<int>* dp = &vector_1;
    vector<int>* dp_pre = &vector_2;

    for(int no = 1; no <= number_of_objects; no++){
        for(int c = 0; c <= capacity; c++){

            bool can_take = weights[no-1] <= c;

            int max_if_take = 0;
            if(can_take){
                max_if_take = costs[no-1] + (*dp_pre)[c - weights[no-1]];
            }
            int max_if_not_take = (*dp_pre)[c];

            (*dp)[c] = max(max_if_take,max_if_not_take);
        }

        //printv(*dp);


        vector<int>* tmp = dp;
        dp = dp_pre;
        dp_pre = tmp;
    }


    output << (*dp_pre)[capacity];

    //cout << memory;

    input.close();
    output.close();
    return 0;
}