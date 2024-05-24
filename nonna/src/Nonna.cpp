#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include "nonna.h"

using namespace std;

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    vector<tuple<unsigned,unsigned>> lines;

    unsigned C, G, M;
    in >> C >> G >> M;
    for(unsigned i=0; i<M; i++) {
        unsigned b, e;
        in >> b >> e;
        lines.push_back({b, e});
    }

    sort(lines.begin(), lines.end());

    unsigned counter = 0;
    for(unsigned current=1; current<lines.size(); current++) {
        unsigned c_dest = get<1>(lines[current]);
        for(int previous=current-1; previous>=0; previous--) {
            if(get<1>(lines[previous]) > c_dest) {counter++;}
        }
    }
    out << counter << endl;
    for(unsigned c=0; c<C; c++) {out << c << " ";}
    out << endl << "***" << endl;
    
    return 0;
}