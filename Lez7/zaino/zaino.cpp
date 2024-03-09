#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <tuple>
#include <climits>

using namespace std;

class Memory{
public:

    vector<map<int,int>> memory;

    explicit Memory(int _number_of_objects){
        memory = vector<map<int,int>>(_number_of_objects,map<int,int>());
    }

    int get(int c, int n){
        auto v = this->memory[n].find(c);
        if (v == memory[n].end()){
            return -1;
        }
        return this->memory[n][c];
    }

    void set(int c, int n, int to_set){
        memory[n][c] = to_set;
    }
};

vector<int> costs;
vector<int> weights;
Memory memory = Memory(0);

int max_value(int number_of_objects, int capacity){


    if(capacity < 0){
        return INT_MIN;
    }

    if(number_of_objects == 0 || capacity == 0){
        return 0;
    }

    int v = memory.get(capacity,number_of_objects-1);

    if(v!=-1){
        return v;
    }

    int max_if_not_takeing = max_value(number_of_objects - 1, capacity);
    int max_if_takeing = costs[number_of_objects - 1] + max_value(number_of_objects - 1, capacity - weights[number_of_objects - 1]);

    v = max(max_if_not_takeing,max_if_takeing);

    memory.set(capacity,number_of_objects-1, v);

    return v;
}


int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    int capacity, number_of_objects;

    input >> capacity >> number_of_objects;

    costs = vector<int>();
    weights = vector<int>();

    costs.reserve(number_of_objects);
    weights.reserve(number_of_objects);

    for(int i=0; i<number_of_objects; i++){
        int weight,cost;
        input >> weight >> cost;
        weights.push_back(weight);
        costs.push_back(cost);
    }

    memory = Memory(number_of_objects);

    int result = max_value(number_of_objects, capacity);

    output << result;

    input.close();
    output.close();
    return 0;
}