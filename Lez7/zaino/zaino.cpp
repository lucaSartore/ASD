#include <fstream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <limits.h>

using namespace std;

class CellRef{
  public:
    int capacity;
    int number_of_objects;

    CellRef(int _capacity, int _number_of_objects){
      capacity = _capacity;
      number_of_objects = _number_of_objects;
    }

    bool operator==(CellRef const & other) const{
      return other.capacity == this->capacity && other.number_of_objects == this->number_of_objects;
    }
};

namespace std{
  template<>
  struct hash<CellRef>
  {
      std::size_t operator()(const CellRef c) const
      {
          int result = 0;
          result = result ^ (int)hash<int>()(c.capacity);
          result = result ^ (int)hash<int>()(c.number_of_objects);
          return (size_t) result;
      }
  };
}
int max_value(vector<int> & costs, vector<int> & weight, unordered_map<CellRef,int> & memory, int number_of_object, int capacity){
    if(capacity < 0){
        return INT_MIN;
    }

    if(number_of_object == 0 || capacity == 0){
        return 0;
    }

    if (memory.find(CellRef(capacity,number_of_object)) != memory.end()){
        return memory[CellRef(capacity,number_of_object)];
    }

    int max_if_not_tacking = max_value(costs,weight,memory,number_of_object-1,capacity);
    int max_if_tacking = costs[number_of_object-1] + max_value(costs,weight,memory,number_of_object-1,capacity - weight[number_of_object-1]);

    int val = max(max_if_not_tacking,max_if_tacking);

    memory[CellRef(capacity,number_of_object)] = val;

    return val;
}


int main(){
  ifstream input("input.txt");
  ofstream output("output.txt");

  unsigned int capacity, number_objects;

  input >> capacity >> number_objects;

  vector<int> costs = vector<int>();
  vector<int> weights = vector<int>();
  costs.reserve(number_objects);
  weights.reserve(number_objects);

  for(int i=0; i<number_objects; i++){
    unsigned int weight,cost;
    input >> weight >> cost;
    weights.push_back(weight);
    costs.push_back(cost);
  }

  unordered_map<CellRef,int> memory;

  int result = max_value(costs, weights, memory, number_objects, capacity);

    output << result;

    input.close();
    output.close();

  return 0;
}
