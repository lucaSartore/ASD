#include <unordered_map>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <climits>
#include <vector>
using namespace std;


class SetOfElements{
public:

    vector<int> set_of_elements;
    unordered_map<int,int> reverse_set_of_elements;

    explicit SetOfElements(vector<int> & elements){
        set<int> s = set<int>(elements.begin(),elements.end());
        set_of_elements = vector<int>(s.begin(),s.end());
        sort(set_of_elements.begin(), set_of_elements.end());

        reverse_set_of_elements = unordered_map<int,int>();

        for(int i=0; i<set_of_elements.size(); i++){
            reverse_set_of_elements[set_of_elements[i]] = i;
        }
    }

    int id_to_number(int id)const {
        return  set_of_elements[id];
    }

    int number_to_id(int number)const{
        return reverse_set_of_elements.find(number)->second;
    }
};

template<typename T>
ostream& operator<< (ostream& os, vector<T>* v){
    os << "[";
    for(auto x: *v){
        os << x << "\t";
    }
    os << "]";
    return os;
}


int main(){

    ifstream input("input.txt");
    ofstream output("output.txt");

    unordered_map<int,int> max_value_to_max_sum = unordered_map<int,int>();

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

    int distinct_max_values = soe.set_of_elements.size();

    vector<int>elements_ids = vector<int>(elements.size(),0);

    transform(elements.begin(),elements.end(),elements_ids.begin(),[&soe](int x){return  soe.number_to_id(x);});



    output << max_sum_by_max_id_prev->end().operator--().operator*();

    input.close();
    output.close();
}