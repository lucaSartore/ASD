#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

//#define DEBUG

using namespace std;

enum Direction{
    LEFT,
    RIGHT
};

void operator>>(ifstream& file, Direction& direction){
    char c; file >> c;
    if(c == 'd' || c == 'D'){
        direction = RIGHT;
    }else if(c == 's' || c == 'S'){
        direction = LEFT;
    }else if(c == ' ' || c == '\n'){
        file >> direction;
    }else{
        throw runtime_error("invalid format");
    }
}

ostream& operator<<(ostream& os, Direction direction){
    if(direction == LEFT){
        os << "<";
    }else{
        os << ">";
    }
    return os;
}
/////////// global variables //////////////
vector<Direction> triangles;
vector<bool> is_splittable;
vector<bool> is_splittable_odd;
vector<bool> is_splittable_even;
vector<int> splittable_odd_cumulate;
vector<int> splittable_even_cumulate;
int len;

bool is_simplifiable_fn(int start, int end) {


    //empty slice is always simplifiable
    if(end+1 == start){
        return true;
    }

    assert(end >= start);

    int len_slice = end + start + 1;

    // odd sequences can't be simplified
    if (len_slice % 2 == 1) {
        return false;
    }

    // a sequence delimitated by a non outward-facing triangle is always simplifiable
    if ((triangles[start] == RIGHT && start != 0) || (triangles[end] == LEFT && end != len-1)) {
        return true;
    }

    // otherwise I need to check if the sequence can be split into two simplifiable sequences

    // if start is even I must split at an odd position, otherwise I must
    // split at an even position
    vector<int> *splittable_cumulated;
    if (start % 2 == 0) {
        splittable_cumulated = &splittable_odd_cumulate;
    } else {
        splittable_cumulated = &splittable_even_cumulate;
    }

    int possible_split_count = (*splittable_cumulated)[end - 1] - (*splittable_cumulated)[start];

    return possible_split_count != 0;
}

int main(){

    ////////////////// input the files /////////////////////

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> len;

    triangles = vector<Direction>();
    triangles.reserve(len);

    for(int i=0; i<len; i++){
        Direction d; input >> d;
        triangles.push_back(d);
    }
    cout << endl << endl;

    //////////// pre processing ////////////////////////
    //find the indexes where the triangles can be split into two simplifiable queue of triangles
    // notation:
    // given the indexes: 0 1 2 3
    // splitting at index 1 means having:   0 1   an   2 3 as result of the split

    is_splittable = vector<bool>();
    is_splittable.reserve(len-1);

    for(int i=0; i<len-1; i++){
        if(triangles[i] == LEFT && triangles[i+1] == RIGHT){
            is_splittable.push_back(true);
        }else{
            is_splittable.push_back(false);
        }
    }

    /// even and odd

    is_splittable_even = vector<bool>();
    is_splittable_even.reserve(len-1);
    is_splittable_odd = vector<bool>();
    is_splittable_odd.reserve(len-1);

    for(int i=0; i<len-1; i++){
        if(i%2){
            // odd
            is_splittable_odd.push_back(is_splittable[i]);
            is_splittable_even.push_back(false);
        }else{
            // even
            is_splittable_even.push_back(is_splittable[i]);
            is_splittable_odd.push_back(false);
        }
    }

    /// cumulated value ///

    splittable_even_cumulate = vector<int>();
    splittable_even_cumulate.reserve(len-1);
    splittable_odd_cumulate = vector<int>();
    splittable_odd_cumulate.reserve(len-1);

    int prev_odd = 0;
    int prev_even = 0;
    for(int i=0; i<len-1; i++){
        prev_odd += is_splittable_odd[i];
        splittable_odd_cumulate.push_back(prev_odd);

        prev_even += is_splittable_even[i];
        splittable_even_cumulate.push_back(prev_even);
    }

    ////////////////// calculation /////////////////////////

    vector<int> possible_last = vector<int>();

    for(int i=0; i<len; i++){
        bool left = is_simplifiable_fn(0,i-1);
        bool right = is_simplifiable_fn(i+1,len-1);
        if(left && right){
            possible_last.push_back(i);
        }
    }
    ////////////// print //////////////////
#ifdef DEBUG
    cout << "triangles            : ";
    for(auto e: triangles){
        cout << e << " ";
    }
    cout << endl;

    cout << "can_be_split         : ";
    for(auto e: is_splittable){
        cout << e << " ";
    }
    cout << endl;


    cout << "can_be_split_odd     : ";
    for(auto e: is_splittable_odd){
        cout << e << " ";
    }
    cout << endl;


    cout << "can_be_split_even    : ";
    for(auto e: is_splittable_even){
        cout << e << " ";
    }
    cout << endl;


    cout << "cumulate_split_even  : ";
    for(auto e: splittable_even_cumulate){
        cout << e << " ";
    }
    cout << endl;

    cout << "cumulate_split_odd   : ";
    for(auto e: splittable_odd_cumulate){
        cout << e << " ";
    }
    cout << endl;

#endif

    ////////////////// output files /////////////////////////

    output << possible_last.size() << endl;
    for(auto elem: possible_last){
        output << elem << " ";
    }
    output.close();
    input.close();
}