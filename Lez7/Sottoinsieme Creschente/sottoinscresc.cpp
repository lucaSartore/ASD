//
// Created by lucas on 09/03/2024.
//

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <climits>
#include <vector>
using namespace std;

#define UNINIT INT_MIN

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

class Memory{
public:

    vector<unordered_map<int,int>> memory;
    int n_items;
    int n_distinct_items;

    Memory(int _n_items, int _n_distinct_items ){
        n_items = _n_items;
        n_distinct_items = _n_distinct_items;
        memory = vector<unordered_map<int,int>>(_n_items,unordered_map<int,int>());
    }

    int get(int item, int distinct_item_id) const{
        auto v = memory[item].find(distinct_item_id);
        if(v == memory[item].end()){
            return UNINIT;
        }
        return v->second
    }

    void set(int item, int distinct_item_id, int value){
        memory[item][distinct_item_id] = value;
    }
};

ostream& operator<<(ostream& os, const Memory& mem){
    for(int c = 0; c < mem.n_distinct_items; c++){
        for(int n=0; n<mem.n_items; n++){
            int v = mem.get(c,n);
            if(v == UNINIT){
                os << "U" << "\t";
            }else{
                os << v << "\t";
            }
        }
        os << endl;
    };
    return os;
}


int get_max_value(int number, unordered_map<int,int> & max_value_to_max_sum){
    auto v = max_value_to_max_sum.find(number);
    if(v != max_value_to_max_sum.end()){
        return v->second;
    }
    return 0;
}

// item_index: pointer to the array of values
// id_max_value:  item id to the array of distinct values
// soe.id_to_number(id_max_value): the max value an item can have to be included in the valuation (included)
// index means it points to the array of inputs
// id means it points to the sorted array of unique dimensions
int get_max_sum(int id_max_value, int item_index, Memory & m, SetOfElements const & soe, vector<int> const & array){

    if(id_max_value == -1){
        return 0;
    }

    int item_value = array[item_index];
    int max_value = soe.id_to_number(id_max_value);
    int item_id = soe.number_to_id(item_value);


    bool can_take = item_value <= max_value;

    if(item_index == 0){
        if(can_take){
            m.set(item_index,id_max_value,item_value);
            return item_value;
        } else{
            m.set(item_index,id_max_value,0);
            return 0;
        }
    }

    int v = m.get(item_index, id_max_value);
    if(v != UNINIT){
        return v;
    }

    int max_if_take = 0;
    if(can_take){
        max_if_take = item_value + get_max_sum(item_id-1,item_index-1,m,soe,array);
    }
    int max_if_not_take = get_max_sum(id_max_value,item_index-1,m,soe,array);

    int value = max(max_if_not_take,max_if_take);

    m.set(item_index,id_max_value,value);
    return value;
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

    Memory m = Memory(n_object,soe.set_of_elements.size());

    output << get_max_sum(soe.set_of_elements.size()-1,n_object-1,m,soe,elements);
    //cout << m << endl;

    input.close();
    output.close();
}