#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <tuple>
#include <climits>
#include <iostream>

using namespace std;

class Memory{
public:

    int* memory;
    int n_items;
    int capacity;


    explicit Memory(int _n_items, int _capacity){
        memory = new int[_n_items * _capacity];
        n_items = _n_items;
        capacity = _capacity;
    }

    int get(int c, int n) const{
        if(c<0){
            return INT_MIN;
        }
        return memory[c*n_items + n];
    }

    void set(int c, int n, int to_set) {
        memory[c * n_items + n] = to_set;
    }
};

ostream& operator<<(ostream& os, const Memory& mem){
    for(int c = 0; c < mem.capacity; c++){
        for(int n=0; n<mem.n_items; n++){
            os << mem.get(c,n) << "\t";
        }
        os << endl;
    };
    return os;
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

    Memory memory = Memory(number_of_objects+1,capacity+1);


    for(int no = 0; no <= number_of_objects; no++){
        for(int c = 0; c <= capacity; c++){

            if(c == 0 || no == 0){
                memory.set(c,no,0);
                continue;
            }

            int max_if_not_takeing = memory.get(c,no-1);
            int max_if_takeing = costs[no - 1] + memory.get(c - weights[no - 1],no-1);

            memory.set(c,no,max(max_if_not_takeing,max_if_takeing));
        }
    }

    output << memory.get(capacity,number_of_objects);

    //cout << memory;

    input.close();
    output.close();
    return 0;
}