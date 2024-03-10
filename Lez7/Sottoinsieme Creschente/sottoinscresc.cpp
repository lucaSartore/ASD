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
    //elements_ids.reserve(elements.size(),0);

    transform(elements.begin(),elements.end(),elements_ids.begin(),[&soe](int x){return  soe.number_to_id(x);});

    vector<int> m1(distinct_max_values,0);
    vector<int> m2(distinct_max_values,0);

    vector<int>* max_sum_by_max_id = &m1;
    vector<int>* max_sum_by_max_id_prev = &m2;

    //cout << max_sum_by_max_id_prev << endl;

    for(int i=0; i<n_object; i++){

        int item_id = elements_ids[i];

        for(int id_max_value = 0; id_max_value<distinct_max_values; id_max_value++){

            bool can_take = item_id <= id_max_value;

            int max_value_if_not_take = (*max_sum_by_max_id_prev)[id_max_value];

            int max_value_if_take = 0;
            if(can_take){

                int item_value = soe.id_to_number(item_id);


                max_value_if_take = item_value + (*max_sum_by_max_id_prev)[item_id];

            }

            int max_value = max(max_value_if_take,max_value_if_not_take);

            (*max_sum_by_max_id)[id_max_value] = max_value;
        }

        //cout << max_sum_by_max_id << endl;

        vector<int>* tmp = max_sum_by_max_id;
        max_sum_by_max_id = max_sum_by_max_id_prev;
        max_sum_by_max_id_prev = tmp;
    }

    output << max_sum_by_max_id_prev->end().operator--().operator*();

    input.close();
    output.close();
}