
#include <iostream>
#include <fstream>
#include <assert.h>
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

    bool operator==(CellRef& other){
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
int max_value(vector<int> & costs, vector<int> & weigth, unordered_map<tuple<int,int>,int> & memory, int number_of_object, int capacity){
  if(capacity < 0){
    return INT_MIN; 
  }
  if(number_of_object == 0 && capacity == 0){
    return 0;
  }
  int result = memory[tuple<int,int>(number_of_object,capacity)];

  return result;
}


int main(){
  ifstream input("input.txt");

  unsigned int capaciry, number_objects;

  input >> capaciry >> number_objects;

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

  unordered_map<tuple<int,int>,int> memory;

  int result = max_value(costs,weights, memory, number_objects, capaciry)

  return 0;
}
