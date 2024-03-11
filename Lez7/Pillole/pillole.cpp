#include <unordered_map>
#include <cctype>
#include <iostream>
#include <fstream>

using namespace std;

class Args{
public:
    uint16_t half_pillows;
    uint16_t full_pillows;

    Args(uint16_t _half_pillows, uint16_t _full_pillows){
        half_pillows = _half_pillows;
        full_pillows = _full_pillows;
    }

    Args operator-(Args const& other)const{
        int new_half_pillows = half_pillows - other.half_pillows;
        int new_full_pillows = full_pillows - other.full_pillows;
        if(new_full_pillows < 0 || new_half_pillows < 0){
            new_half_pillows = 0;
            new_full_pillows = 0;
            throw exception();
        }
        return Args{
                (uint16_t)new_half_pillows,
                (uint16_t)new_full_pillows
        };
    }
    Args operator+(Args const& other)const{
        return Args{
               (uint16_t)(half_pillows + other.half_pillows),
               (uint16_t)(full_pillows + other.full_pillows)
        };
    }


    bool operator==(Args const & other)const{
        return half_pillows == other.half_pillows && full_pillows == other.full_pillows;
    }
};

hash<uint32_t> hasher;

namespace std{
    template<>
    class hash<Args>{
    public:
        size_t operator()(Args const & item)const{
            uint32_t to_hash = item.full_pillows;
            to_hash <<= 16;
            to_hash += item.half_pillows;
            return hasher(to_hash);
        }
    };
}

unordered_map<Args,uint64_t> cache;

uint64_t get_n_combinations(Args args){

    if(args == Args(0,0)){
        return 1;
    }

    auto val = cache.find(args);
    if(val != cache.end()){
        return val->second;
    }

    uint64_t c1 = 0, c2 = 0;
    try{
        c1 = get_n_combinations(args - Args(0,1) + Args(1,0));
    }catch (exception &e){}
    try{
        c2 = get_n_combinations(args - Args(1,0));
    }catch (exception &e){}


    uint64_t v = c1+c2;

    cache[args] = v;

    return v;
}

int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    int n_pillows;
    cache = unordered_map<Args,uint64_t>();

    input >> n_pillows;

    uint64_t result = get_n_combinations(Args(0,n_pillows));

    output << result;

    input.close();
    output.close();
    return 0;
}