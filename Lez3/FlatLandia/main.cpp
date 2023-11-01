#include <iostream>
#include <fstream>
#include <vector>

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


int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");

    int len; input >> len;

    vector<Direction> triangles = vector<Direction>();
    triangles.reserve(len);
    for(int i=0; i<len; i++){
        Direction d; input >> d;
        triangles.push_back(d);
        cout << d;
    }




}