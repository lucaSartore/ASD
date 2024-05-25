#ifndef LOCAL 
#include "nonna.h"
#endif // !LOCAL 

#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>

using namespace std;

class Centrino {
public:
	int position;
	int id;
	vector<int> gomitoli;
	float ordering;
	int average;
	int variance;  // this is useful to know as a high variance means that it's more complex to correctly find the optimal placement for this centrino

	explicit Centrino(int _id) {
		position = _id;
		id = _id;
		gomitoli = vector<int>();
	}

	void add_gomitolo(int gomitolo) {
		gomitoli.push_back(gomitolo);
	}

	void calculate_median() {
		int size = gomitoli.size();
		if (size == 0){
			ordering = 0;
		}
		else if (size % 2 == 0) {
			ordering = (gomitoli[size / 2] + gomitoli[size / 2 - 1]) / 2.0;
		}
		else {
			ordering = gomitoli[size / 2];
		}
	}

	void calculate_average() {

		int sum = 0;
		for(int e: gomitoli){
			sum += e;
		}
		average = ((float)sum) / gomitoli.size();
		ordering = average;
	}
	
	// calculate the variance of all the gomitoli connected to this centrino
	void calculate_variance() {
		float sum = 0;
		for(int e: gomitoli){
			sum += pow(e - average, 2);
		}
		variance = sum / gomitoli.size();
	}

	bool operator<(Centrino& other) {
		return ordering < other.ordering ;
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

		for (int i = 0; i < num_centrini; i++) {
			sort(centrini[i].gomitoli.begin(), centrini[i].gomitoli.end());
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

	void greedy_ordering(bool use_median = false) {
	
		if (use_median) {
			for (auto& c : centrini) {
				c.calculate_median();
			}
		}
		else {
			for (auto& c : centrini) {
				c.calculate_average();
			}
		}

		sort(centrini.begin(), centrini.end());

		restore();
	}

	// return a list of random continuous intervals
	// the list is returned when the next random interval intersects an already added interval
	// TODO: make this optionally take in a minimum and maximum value, also try making the interval lenght an exponential function to make long ones rarer
	void get_random_intervals() {
		std::vector<std::pair<int, int>> random_intervals;
    	pairs.reserve(num_centrini);
		
		// seed the pseudo random number generator
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		for (int i = 0; i < num_centrini; ++i) {
			int start_index = std::rand() % (num_centrini + 1);
			int end_index = std::rand() % (num_centrini + 1);

			pairs.emplace_back(start_index, end_index);
		}

		return random_intervals;
	}

	// return a list of continuous intervals with the highest variance in them (ordered from worse to best)
	// the list is returned when the next worse interval intersects an already added interval
	void get_worse_intervals() {
		std::vector<std::pair<int, int>> worse_intervals;
    	pairs.reserve(num_centrini);
		
		// TODO: actually implement this, this algorithm was explained during the course

		return worse_intervals;
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
	int current_best = solution.crossing;


	output << solution;

	solution.greedy_ordering();
	solution.calculate_crossing();

	if (solution.crossing < current_best) {
		current_best = solution.crossing;
		output << solution;
	}

	
	solution.greedy_ordering(true);
	solution.calculate_crossing();

	if (solution.crossing < current_best) {
		current_best = solution.crossing;
		output << solution;
	}

	return 0;
}
