#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <climits>
#include <vector>
using namespace std;

using IterType = map<int,int,greater<int>>::iterator;

class Memory{
public:
    map<int,int,greater<int>> memory;

    explicit Memory(){
        memory = map<int,int,greater<int>>();
    }

    void set(int last_item, int max_score){
        memory[last_item] = max_score;
    }

    int get_max_score_for_last_item_lower_or_equal_of(int last_item){
        auto v =  memory.lower_bound(last_item);
        if(v == memory.end()){
            // if no item is found, it means i need to start form zero
            return 0;
        }
        return v->second;
    }

    // I delete the elements that I don't need to keep, as there
    // are better options for the score...
    // An element is deleted if there is a tuple with a score that is greater or equal, but has a lower max_value
    void clear_memory(int filter_for_elements_greater_then, int remove_element_with_sore_lower_then){
        vector<int> to_remove = vector<int>();

        auto start = memory.find(filter_for_elements_greater_then);
        auto start_reverse = reverse_iterator<IterType>(start)++;

        for(auto e = start_reverse; e != memory.rend(); e++){
            if(e->second < remove_element_with_sore_lower_then){
                to_remove.push_back(e->first);
            }else{
                // once I find an element that can't be removed I can be shore that the next can't be removed either
                break;
            }
        }

        for(auto e: to_remove){
            memory.erase(e);
        }
    }

    int get_max_score(){
        int max_so_far = 0;
        for(auto e: memory){
            max_so_far = max(max_so_far,e.second);
        }
        return max_so_far;
    }
};

template<typename T, typename  E, typename  F>
ostream& operator<< (ostream& os, map<T,E,F>& v){
    os << "[";
    for(pair<const T, E> x: v){
        os << "[" << x.first << ": " << x.second << "]\t";
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

    Memory memory = Memory();

    for(int i=0; i<n_object; i++){
        int x;
        input >> x;

        int possible_score = memory.get_max_score_for_last_item_lower_or_equal_of(x) + x;
        int max_score_otherwise = memory.get_max_score_for_last_item_lower_or_equal_of(x-1);
        if(possible_score > max_score_otherwise){
            memory.set(x,possible_score);
            memory.clear_memory(x,possible_score);
        }
        //cout << memory.memory << endl;
    }

    output << memory.get_max_score();

    input.close();
    output.close();
}