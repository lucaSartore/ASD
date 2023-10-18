#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

enum BoundKind{START,END};

struct IntervalBound{
public:
    BoundKind kind;
    int time;

    bool operator< (IntervalBound& other) const{return time <  other.time;};
    bool operator<=(IntervalBound& other) const{return time <= other.time;};
    bool operator> (IntervalBound& other) const{return time >  other.time;};
    bool operator>=(IntervalBound& other) const{return time >= other.time;};

    static tuple<IntervalBound,IntervalBound> from_file(ifstream& file){
        int t1, t2;
        file >> t1 >> t2;
        return make_tuple(IntervalBound(START,t1),IntervalBound(END,t2));
    }

    IntervalBound(BoundKind kind, int time): kind(kind), time(time){};
};

int main(){

    ifstream input("input.txt");
    ofstream output("output.txt");

    int n;
    input >> n;

    vector<IntervalBound> bounds;
    bounds.reserve(2*n);

    for(int i=0; i<n; i++){
        auto tuple = IntervalBound::from_file(input);
        bounds.push_back(get<0>(tuple));
        bounds.push_back(get<1>(tuple));
    }

    sort(bounds.begin(), bounds.end());

    int open = 0;
    int max_time = 0;
    int last_close = -1;

    int max_start = -1;
    int max_end = -1;

    for(auto e: bounds){
        if(e.kind == START){
            if(open == 0 && last_close != -1){
                if(max_time < e.time - last_close){
                    max_time = e.time - last_close;
                    max_start = last_close;
                    max_end = e.time;
                }
            }
            open++;
        }else{
            open--;
            if(open == 0){
                last_close = e.time;
            }
        }
    }
    if(max_time == 0){
        output << 0;
    }else{
        output << max_start << " " << max_end;
    }

    output.close();
    input.close();
}