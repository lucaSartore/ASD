//
// Created by lucas on 09/03/2024.
//

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
using namespace std;

class SetOfElements{

public:

    vector<int> set_of_elements;

    SetOfElements(vector<int> & elements){
        set<int> s = set<int>(elements.begin(),elements.end());
        set_of_elements = vector<int>(s.begin(),s.end());
        sort(set_of_elements.begin(), set_of_elements.end());
    }
};

int get_max_value(int number, unordered_map<int,int> & max_value_to_max_sum){
    auto v = max_value_to_max_sum.find(number);
    if(v != max_value_to_max_sum.end()){
        return v->second;
    }
    return 0;
}

int main(){

    ifstream input("input.txt");
    ofstream output("output.txt");

    unordered_map<int,int> max_value_to_max_sum = unordered_map<int,int>();

    int max_so_far = 0;

    int n_object;
    input >> n_object;

    vector<int> elements = vector<int>();
    elements.reserve(n_object);

    for(int i=0; i<n_object; i++){
        int x;
        input >> x;
        elements.push_back(x);
    }

    SetOfElements soe = SetOfElements(elements);

    input.close();
    output.close();
}