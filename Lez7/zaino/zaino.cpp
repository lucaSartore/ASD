#include <fstream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <limits.h>

using namespace std;

class CellRef{
  public:
    uint32_t capacity;
    uint32_t number_of_objects;

    CellRef(uint32_t _capacity, uint32_t _number_of_objects){
      capacity = _capacity;
      number_of_objects = _number_of_objects;
    }
};

class CellRefPack{
public:
    uint64_t values;

    CellRefPack(CellRef p){
        values = p.capacity;
        values <<= 32;
        values |= p.number_of_objects;
    }

    bool operator==(CellRefPack const & other) const{
        return this->values == other.values;
    }
};

hash<uint64_t> hasher = hash<uint64_t>();

namespace std{
  template<>
  struct hash<CellRefPack>
  {
      std::size_t operator()(const CellRefPack c) const
      {
          return hasher(c.values);
      }
  };
}

int max_value(vector<int> & costs, vector<int> & weight, unordered_map<CellRefPack,int> & memory, int number_of_objects, int capacity){


    if(capacity < 0){
        return INT_MIN;
    }

    if(number_of_objects == 0 || capacity == 0){
        return 0;
    }

    CellRefPack cr = CellRef(capacity, number_of_objects);

    auto value   = memory.find(cr);
    if (value != memory.end()){
        auto x = *value;
        return get<1>(*value);
    }

    int max_if_not_takeing = max_value(costs, weight, memory, number_of_objects - 1, capacity);
    int max_if_takeing = costs[number_of_objects - 1] + max_value(costs, weight, memory, number_of_objects - 1, capacity - weight[number_of_objects - 1]);

    int val = max(max_if_not_takeing,max_if_takeing);

    memory[cr] = val;

    return val;
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

    unordered_map<CellRefPack,int> memory;
    int result = max_value(costs, weights, memory, number_of_objects, capacity);

    output << result;

    input.close();
    output.close();
    return 0;
}