#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <unordered_set>

using namespace std;


class NumbersSet{
    vector<int> numbers;
public:
    // fill up the set with all the elements that are linked to the original
    // index by following the rotation.
    NumbersSet(vector<int> &data, vector<int> &position_map, int index_start){
        numbers = vector<int>();
        int start_value = data[index_start];

        numbers.push_back(start_value);
        int index = index_start;
        while (true){
            // calculate the next index
            index = position_map[index];
            // get the new element
            int new_element = data[index];
            // if we closed the loop we are done
            if (new_element == start_value) break;
            // otherwise add the new element to the set
            numbers.push_back(new_element);
        }
    }

    int total_moves(){
        return numbers.size() - 1;
    }

    int cost_with_swap(){
        return sum() + min() + len() + 1;
    }

    int cost_without_swap(){
        return sum() + min()*(len()-2);
    }

    int len(){
        return numbers.size();
    }

    int sum(){
        int sum = 0;
        for(int i=0; i<numbers.size(); i++){
            sum += numbers[i];
        }
        return sum;
    }

    int min(){
        int min = numbers[0];
        for(int i=1; i<numbers.size(); i++){
            if (numbers[i] < min) min = numbers[i];
        }
        return min;
    }

    vector<int>& getNumbers(){
        return numbers;
    }
};

// print function for NumbersSet
ostream& operator<<(ostream& os, NumbersSet& ns){
    vector<int> &numbers = ns.getNumbers();
    cout << "NumbersSet: ";
    for(int i=0; i<numbers.size(); i++){
        os << numbers[i] << " ";
    }
    cout << endl;
    return os;
}

int main(){

    //////////////////// INPUT ///////////////////////

    ifstream input("input.txt");
    ofstream output("output.txt");

    int n;
    input >> n;

    vector<int> data;
    data.reserve(n);

    vector<int> position_mapp;
    position_mapp.reserve(n);

    for(int i=0; i<n; i++) position_mapp.push_back(0);

    for(int i=0; i<n; i++){
        int tmp;
        input >> tmp;
        data.push_back(tmp);
        position_mapp[tmp-1] = i;
    }

    //////////////////// CREATE SETS ///////////////////////

    unordered_set<int> already_used_numbers = unordered_set<int>();
    vector<NumbersSet> sets = vector<NumbersSet>();
    for(int i=0; i<n; i++){
        int starting_number = data[i];
        // if we already used this number we can skip it
        if (already_used_numbers.find(starting_number) != already_used_numbers.end()) continue;
        // get the set that starts at position i
        NumbersSet ns(data, position_mapp, i);
        // insert the elements of the set in the already used numbers
        vector<int> &numbers = ns.getNumbers();
        for(int & number : numbers){
            already_used_numbers.insert(number);
        }
        sets.push_back(std::move(ns));
        // print the found set
        //cout << ns << endl;
    }

    //////////////////// CALCULATE VALUES ///////////////////////

    int total_moves = 0;
    int total_cost = 0;

    // iter over sets
    for(NumbersSet &set : sets){
        cout << set << endl;
        total_moves += set.total_moves();
        total_cost += min(set.cost_with_swap(), set.cost_without_swap());
    }

    //////////////////// OUTPUT ///////////////////////


    //cout << "Total moves: " << total_moves << endl;
    //cout << "Total cost: " << total_cost << endl;

    output << total_moves << " " << total_cost << endl;

    input.close();
    output.close();
}