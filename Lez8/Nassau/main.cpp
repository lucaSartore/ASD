#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iomanip>
using namespace std;

class Option{
public:
    int n_vessel;
    int n_damaged_vessel;
    int n_frigate;

    bool operator==(Option const & other) const{
        return n_vessel == other.n_vessel &&
                n_damaged_vessel == other.n_damaged_vessel &&
                n_frigate == other.n_frigate;
    }

    Option(int _n_vessel, int _n_damaged_vessel, int _n_frigate){
        n_vessel = _n_vessel;
        n_damaged_vessel = _n_damaged_vessel;
        n_frigate = _n_frigate;
    }
    double get_power() const{
        return n_frigate * (n_vessel + n_damaged_vessel);
    }

    double get_probability_of_hitting_frigate(){
        if(n_frigate <= 0) return 0;
        return ((double)n_frigate)/(n_frigate + n_damaged_vessel + n_vessel);
    }

    double get_probability_of_hitting_vessel(){
        if(n_vessel <= 0) return 0;
        return ((double)n_vessel)/(n_frigate + n_damaged_vessel + n_vessel);
    }

    double get_probability_of_hitting_damaged_vessel(){
        if(n_damaged_vessel <= 0) return 0;
        return ((double)n_damaged_vessel)/(n_frigate + n_damaged_vessel + n_vessel);
    }

    Option minus_one_frigate(){
       return Option(
               n_vessel,
               n_damaged_vessel,
               n_frigate - 1
               );
    }

    Option minus_one_vessel(){
        return Option(
                n_vessel - 1,
                n_damaged_vessel + 1,
                n_frigate
        );
    }
    Option minus_one_damaged_vessel(){
        return Option(
                n_vessel,
                n_damaged_vessel - 1,
                n_frigate
        );
    }
};

hash<size_t> hasher = hash<size_t>();
namespace std{

    template<>
    class hash<Option>{
    public:
        size_t operator()(Option const & option)const{
            size_t n1 = option.n_frigate;
            size_t n2 = option.n_damaged_vessel;
            size_t n3 = option.n_vessel;
            n1 <<= 32;
            n2 <<= 16;
            return hasher(n1^n2^n3);
        }
    };
}

void add_to_map(unordered_map<Option,double>& map, Option option, double probability){
    auto x = map.find(option);
    if(x == map.end()){
        map[option] = probability;
    }else{
        map[option] += probability;
    }
}

double pow(double base, int exponent){
    bool is_negative = false;
    if(exponent < 0){
        is_negative = true;
        exponent *= -1;
    }
    double result = 1;
    for(int i=0; i<exponent; i++){
        result *= base;
    }
    if(is_negative){
        result = 1.0/result;
    }
    return result;
}

double calculate_strength(Option starting_position, int n_cannon_balls){
   unordered_map<Option,double> d1 = unordered_map<Option,double>();
   unordered_map<Option,double> d2 = unordered_map<Option,double>();

    unordered_map<Option,double>& current_map = d1;
    unordered_map<Option,double>& next_map = d2;

    current_map[starting_position] = 1.0;

    for(int i=0; i<n_cannon_balls; i++){
       for(auto item: current_map) {
           auto option = item.first;
           auto probability = item.second;

           double probability_hit_frigate = option.get_probability_of_hitting_frigate();
           add_to_map(next_map, option.minus_one_frigate(), probability * probability_hit_frigate);

           double probability_hit_vessel = option.get_probability_of_hitting_vessel();
           add_to_map(next_map, option.minus_one_vessel(), probability * probability_hit_vessel);

           double probability_hit_damaged_vessel = option.get_probability_of_hitting_damaged_vessel();
           add_to_map(next_map, option.minus_one_damaged_vessel(), probability * probability_hit_damaged_vessel);
       }
       current_map = next_map;
       next_map = unordered_map<Option,double>();

      // clean current map
      double max_probability = 0.0;
      for(auto & p: current_map){
          max_probability = max(max_probability,p.second);
      }

      vector<Option> to_remove = vector<Option>();

      double threshold = max_probability / pow(10.0,12);

       for(auto & p: current_map){
           if(p.second/max_probability < threshold){
               to_remove.push_back(p.first);
           }
       }

       for(auto & r: to_remove){
           current_map.erase(r);
       }
    }
    double total_power = 0;
    for(auto item: current_map){
       total_power += item.first.get_power()*item.second;
    }

    return total_power;
}

int main(){

    ifstream input("input.txt");
    ofstream output("output.txt");

    int vessel, frigates, cannon_balls;

    input >> vessel >> frigates >> cannon_balls;

    double strength = calculate_strength(Option(vessel,0,frigates),cannon_balls);

    output << scientific << setprecision(10) << strength;

    input.close();
    output.close();
}