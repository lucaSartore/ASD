#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;



class PalindromeOption{

public:

    static const char* string_start;
    static const char* string_end;

    int len;
    const char* start;
    const char* end;
    bool finalized;

    PalindromeOption(const char* start_and_end){
        this->len = 1;
        this->start = start_and_end;
        this->end = start_and_end;
        this->finalized = false;
    }
    PalindromeOption(const char* start, const char* end){
        assert(start+1 == end);
        assert(*start == *end);
        this->len = 2;
        this->start = start;
        this->end = end;
        this->finalized = false;
    }

    static void set_string_bounds(const char* start, const char* end){
        string_start = start;
        string_end = start;
    }

    bool try_grow(){
        // can't grow because of bound reached
        if(start == string_start || end == string_end){
            finalized = true;
            return false;
        }
        // can't grow because world is not palindrome
        if(*(start-1) != *(end+1)){
            finalized = true;
            return false;
        }
        // can grow
        start--;
        end++;
        len+=2;
        return true;
    }

};



class Solution {
public:
    string longestPalindrome(string s) {
        const char* str = s.c_str();
        int len = s.size();

        if(len <= 1){
            return s;
        }

        PalindromeOption::set_string_bounds(str,str+len-1);

        vector<PalindromeOption> options = vector<PalindromeOption>();
        int max_pal_len;

        options.emplace_back(str);
        max_pal_len = 1;
        if(str[0] == str[1]){
            options.emplace_back(str,str+1);
            max_pal_len = 1;
        }

        for(int i=0; i<len-1; i++){





        }


        return "";
    }
};


int main(){



}