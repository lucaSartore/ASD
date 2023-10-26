

#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <functional>

using namespace std;


class Factor{
public:
    int factor;
    int factor_index;

    Factor(int factor, int factor_index){
        this->factor = factor;
        this->factor_index = factor_index;
    }

    bool operator==(const Factor& other) const
    {
        return factor == other.factor;
    }
};

template <>
struct std::hash<Factor>
{
    std::size_t operator()(const Factor& f) const
    {
        return hash<int>()(f.factor);
    }
};

class Solution {
public:

    int numFactoredBinaryTrees(vector<int>& arr) {
        // size of the input
        sort(arr.begin(), arr.end());
        int size = arr.size();

        // create vector where all possible factor are stored based on the index
        vector<vector<tuple<Factor,Factor>>> factors = vector<vector<tuple<Factor,Factor>>>();
        factors.reserve(size);
        for(int i=0; i < size; i++){
            factors.push_back(vector<tuple<Factor,Factor>>());
        }

        // create a set of all available numbers to quickly check if a number is available
        unordered_set<Factor> available_numbers = unordered_set<Factor>();
        for(int i=0; i < size; i++){
            available_numbers.insert(Factor(arr[i], i));
        }

        // find all factors
        for(int i=0; i < size; i++){
            for(int j=0; j<i; j++){
                int result = arr[i];
                int dividend = arr[j];

                if(result % dividend != 0){
                    continue;
                }
                int other_dividend_number = result/dividend;

                auto other_dividend_iter = available_numbers.find(Factor(other_dividend_number, 0));

                if(other_dividend_iter == available_numbers.end()){
                    continue;
                }
                Factor other_dividend_factor = *other_dividend_iter;
                Factor dividend_factor = Factor(dividend, j);

                factors[i].push_back(make_tuple(dividend_factor,other_dividend_factor));
            }
        }

        unsigned long long int  combination = 0;
        for(int i=0; i<size; i++){
            combination += numFactoredBinaryTreesRecursive(arr,i,factors);
            combination %= 109 + 7;
        }

        return combination;
    }

    unsigned long long int  numFactoredBinaryTreesRecursive(vector<int>& arr, int index, vector<vector<tuple<Factor,Factor>>>& factors){
        // start from one combination... this one
        unsigned long long int combinations = 1;
        for(auto factor_tuple: factors[index]){
            auto first = get<0>(factor_tuple);
            auto second = get<1>(factor_tuple);

            combinations += numFactoredBinaryTreesRecursive(arr,first.factor_index,factors) - 1;
            combinations %= 109 + 7;

            combinations += numFactoredBinaryTreesRecursive(arr,second.factor_index,factors);
            combinations %= 109 + 7;
        }

        return combinations;
    }

};


int main(){
    auto v = vector<int>{45,42,2,18,23,1170,12,41,40,9,47,24,33,28,10,32,29,17,46,11,759,37,6,26,21,49,31,14,19,8,13,7,27,22,3,36,34,38,39,30,43,15,4,16,35,25,20,44,5,48};

    Solution s;
    cout << "combinations: " << s.numFactoredBinaryTrees(v) << endl;
}