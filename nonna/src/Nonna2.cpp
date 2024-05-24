#ifndef LOCAL 
#include "nonna.h"
#endif // !LOCAL 

#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

class Centrino {
public:
	int position;
	int id;
	vector<int> gomitoli;

	explicit Centrino(int _id) {
		position = _id;
		id = _id;
		gomitoli = vector<int>();
	}

	void add_gomitolo(int gomitolo) {
		gomitoli.push_back(gomitolo);
	}
};

class Line {
public:
	Centrino* centrino;
	int gomitolo;

	Line(Centrino* _centrino, int _gomitolo) {
		centrino = _centrino;
		gomitolo = _gomitolo;
	}

	bool operator< (Line& other) {

		if (this->centrino->position != other.centrino->position) {
			return this->centrino->position < other.centrino->position;
		}
		return this->gomitolo < other.gomitolo;
	}
};

ostream& operator<<(ostream& os, Line& line) {
	os << line.centrino->id << " - " << line.gomitolo;
	return os;
}

class Solution {
public:
	vector<Centrino> centrini;
	vector<Line> lines;
	int num_centrini;
	int num_gomitoli;
	int num_lines;



	explicit Solution(ifstream &fs) {
		fs >> num_centrini >> num_gomitoli >> num_lines;

		for (int i = 0; i < num_centrini; i++) {
			centrini.emplace_back(i);
		}

		for (int i = 0; i < num_lines; i++) {
			int gomitolo, centrino;
			fs >> centrino >> gomitolo;
			centrini[centrino].add_gomitolo(gomitolo);
			lines.emplace_back(&centrini[centrino], gomitolo);
		}
	}

	int calculate_crossing() {

	    sort(lines.begin(), lines.end());

		unsigned counter = 0;

		for(unsigned current=1; current<num_lines; current++) {
			unsigned c_dest = lines[current].gomitolo;
			for(int previous=current-1; previous>=0; previous--) {
				if(lines[previous].gomitolo > c_dest) {
					counter++;
				}
			}
		}

		return counter;
	}
};



int main() {

	ifstream input("input.txt");

	Solution solution = Solution(input);

	cout << solution.calculate_crossing();
}
