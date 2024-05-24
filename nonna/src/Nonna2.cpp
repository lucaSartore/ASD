#ifndef LOCAL 
#include "nonna.h"
#endif // !LOCAL 

#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <numeric>

using namespace std;

class Centrino {
public:
	int position;
	int id;
	vector<int> gomitoli;
	float average;

	explicit Centrino(int _id) {
		position = _id;
		id = _id;
		gomitoli = vector<int>();
	}

	void add_gomitolo(int gomitolo) {
		gomitoli.push_back(gomitolo);
	}

	void calculate_average() {
		int sum = 0;
		for(int e: gomitoli){
			sum += e;
		}
		average = ((float)sum) / gomitoli.size();
	}

	bool operator<(Centrino& other) {
		return average < other.average;
	}
};

class Line;

class Solution;

class Line {
public:
	static Solution* solution;
	int centrino;
	int gomitolo;

	Line(int _centrino, int _gomitolo) {
		centrino = _centrino;
		gomitolo = _gomitolo;
	}

	inline Centrino* get_centrino() const;

	bool operator< (Line& other) {

		if (this->get_centrino()->position != other.get_centrino()->position) {
			return this->get_centrino()->position < other.get_centrino()->position;
		}
		return this->gomitolo < other.gomitolo;
	}
};

Solution* Line::solution = nullptr;

ostream& operator<<(ostream& os, Line& line) {
	os << line.get_centrino()->id << " - " << line.gomitolo;
	return os;
}

class Solution {
public:
	vector<Centrino> centrini;
	vector<int> id_to_position;
	vector<Line> lines;
	int num_centrini;
	int num_gomitoli;
	int num_lines;
	int crossing;



	explicit Solution(ifstream &fs) {
		fs >> num_centrini >> num_gomitoli >> num_lines;

		for (int i = 0; i < num_centrini; i++) {
			centrini.emplace_back(i);
			id_to_position.push_back(i);
		}

		for (int i = 0; i < num_lines; i++) {
			int gomitolo, centrino;
			fs >> centrino >> gomitolo;
			centrini[centrino].add_gomitolo(gomitolo);
			lines.emplace_back(centrino, gomitolo);
		}
	}

	// retore internal references/logic afere notes have been move
	void restore() {
		for (int i = 0; i < num_centrini; i++) {
			centrini[i].position = i;
			id_to_position[centrini[i].id] = i;
		}
	}

	void calculate_crossing() {

	    sort(lines.begin(), lines.end());

		//for (auto& l : lines) {cout << l.get_centrino()->position << ") " << l.get_centrino()->id << " - " << l.gomitolo << endl;}


		unsigned counter = 0;

		for(unsigned current=1; current<num_lines; current++) {
			unsigned c_dest = lines[current].gomitolo;
			for(int previous=current-1; previous>=0; previous--) {
				if(lines[previous].gomitolo > c_dest) {
					counter++;
				}
			}
		}
	
		this->crossing = counter;
	}

	void greedy_ordering() {
		
		for (auto& c : centrini) {
			c.calculate_average();
		}

		sort(centrini.begin(), centrini.end());

		restore();
	}

};


template<typename T, typename E>
T& operator<<(T& os, vector<E> &v) {
	for (T& e : v) {
		os << e << " ";
	}
	return os;
}

template<typename T>
T& operator<<(T& os, Solution & s) {
	os << s.crossing << endl;
	for (auto& c : s.centrini) {
		os << c.id << " ";
	}
	os << endl;
	os << "***" << endl;
	return os;
}

inline Centrino* Line::get_centrino() const{
	int position = solution->id_to_position[centrino];
	return &solution->centrini[position];
}


int main() {

	ifstream input("input.txt");
	ofstream output("output.txt");

	Solution solution = Solution(input);
	Line::solution = &solution;

	solution.calculate_crossing();
	int crossing_old = solution.crossing;


	output << solution;

	solution.greedy_ordering();
	solution.calculate_crossing();

	if (solution.crossing < crossing_old) {
		output << solution;
	}

	return 0;
}
