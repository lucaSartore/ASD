#ifndef LOCAL 
#include "nonna.h"
#endif // !LOCAL 

#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <set>

using namespace std;

class Centrino {
public:
	int position;
	int id;
	vector<int> gomitoli;
	float ordering;
	int average;
    vector<int> go_after;
    vector<int> go_before;
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
    vector<vector<unsigned>> couple_crossings;
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

        vector<unsigned> tmp;
        tmp.resize(num_centrini);
        couple_crossings.resize(num_centrini, tmp);
        for (unsigned c1 = 0; c1 < num_centrini; c1++) {
            for (unsigned c2 = 0; c2 < num_centrini; c2++) {
                couple_crossings[c1][c2] = calculate_couple_crossings(&centrini[c1], &centrini[c2]);
            }
        }
    }

    // retore internal references/logic afere notes have been move
    void restore() {
        for (int i = 0; i < num_centrini; i++) {
            centrini[i].position = i;
            id_to_position[centrini[i].id] = i;
        }
    }

    unsigned calculate_couple_crossings(Centrino *c1, Centrino *c2) {
        if (c1->id == c2->id) { return 0; }
        unsigned counter = 0;
        for (auto g2 = c2->gomitoli.begin(); g2 != c2->gomitoli.end(); g2++) {
            for (auto g1 = c1->gomitoli.begin(); g1 != c1->gomitoli.end(); g1++) {
                counter += (*g2 < *g1) ? 1 : 0;
            }
        }
        return counter;
    }

    void calculate_crossing() {
        unsigned counter = 0;
        set<unsigned> encountered;
        for (unsigned i = 0; i < this->couple_crossings.size(); i++) {
            encountered.insert(this->centrini[i].id);
            for (unsigned j = 0; j < this->couple_crossings.size(); j++) {
                // why using a set here when you could start j from i+1 and with some minor other edits avoid
                // also nothe that a set is not an hash set (that is called unordered_set) and the cost of insertions
                // and deletion is not O(1) bur is O(log(n))
                if (encountered.find(j) == encountered.end()) {
                    counter += this->couple_crossings[this->centrini[i].id][j];
                }
            }
        }
        this->crossing = counter;
    }

    void greedy_ordering(bool use_median = false) {

        if (use_median) {
            for (auto &c: centrini) {
                c.calculate_median();
            }
        } else {
            for (auto &c: centrini) {
                c.calculate_average();
            }
        }

        sort(centrini.begin(), centrini.end());

        restore();
    }
/*
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
    */

    // reorder: begin included, end excluded
    bool topological_order(int begin, int end){

        // reset the vectors
        for(int i=begin; i<end; i++){
            centrini[i].go_after = vector<int>();
            centrini[i].go_before= vector<int>();
        }

        for(int i=begin; i<end; i++) {
            for (int j = begin; j < begin; j++) {
                if( couple_crossings[i][j])
            }
        }
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
