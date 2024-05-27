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
#include <cassert>

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

	void swapp(int i, int j) {
		if (i == j) {
			return;
		}
		int id_i = this->centrini[i].id;
		int id_j = this->centrini[j].id;

		int cost_i_before_j = couple_crossings[id_i][id_j];
		int cost_j_before_i = couple_crossings[id_j][id_i];

		int saved_crossings;
		if (i > j) {
			saved_crossings = - cost_i_before_j + cost_j_before_i;
		}
		else {
			saved_crossings = + cost_i_before_j - cost_j_before_i;
		}
		assert(saved_crossings >= 0);
		this->crossing -= saved_crossings;

		auto tmp = centrini[i];
		centrini[i] = centrini[j];
		centrini[j] = tmp;

		centrini[i].position = i;
		centrini[j].position = j;

		id_to_position[centrini[i].id] = i;
		id_to_position[centrini[j].id] = j;
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
	// return a list of random continuous intervals
	// the list is returned when the next random interval intersects an already added interval
	// TODO: make this optionally take in a minimum and maximum value, also try making the interval lenght an exponential function to make long ones rarer
	vector<pair<int,int>> get_random_intervals() {
		std::vector<std::pair<int, int>> random_intervals;
    	random_intervals.reserve(num_centrini);
		
		// seed the pseudo random number generator
		srand(static_cast<unsigned int>(time(nullptr)));

		for (int i = 0; i < num_centrini; ++i) {
			int start_index = rand() % (num_centrini + 1);
			int end_index = rand() % (num_centrini + 1);

			random_intervals.emplace_back(start_index, end_index);
		}

		return random_intervals;
	}

	// return a list of continuous intervals with the highest variance in them (ordered from worse to best)
	// the list is returned when the next worse interval intersects an already added interval
	std::vector<std::pair<int, int>> get_worse_intervals() {
		std::vector<std::pair<int, int>> worse_intervals;
    	worse_intervals.reserve(num_centrini);
		
		// TODO: actually implement this, this algorithm was explained during the course

		return worse_intervals;
	}

    // reorder: begin included, end excluded
    bool topological_order(int begin, int end){


		end = min(end, num_centrini);

        // reset the vectors
        for(int i=begin; i<end; i++){
            centrini[i].go_after = vector<int>();
            centrini[i].go_before= vector<int>();
        }

		// insert the reordering thing
        for(int i=begin; i<end; i++) {
            for (int j = i+1; j < end; j++) {

				// doto: verigy that these are not inverted 
				int cost_i_before_j = couple_crossings[i][j];
				int cost_j_before_i = couple_crossings[j][i];
				
				if (cost_i_before_j == cost_j_before_i) {
					continue;
				}

				if (cost_i_before_j < cost_j_before_i) {
					centrini[i].go_before.push_back(centrini[j].id);
					centrini[j].go_after.push_back(centrini[i].id);
				}
				else {
					centrini[i].go_after.push_back(centrini[j].id);
					centrini[j].go_before.push_back(centrini[i].id);
				}
            }
        }

		vector<int> node_i_can_pop = vector<int>();
		
		for (int i = begin; i < end; i++) {
			if (centrini[i].go_after.size() == 0) {
				node_i_can_pop.push_back(centrini[i].id);
			}
		}

		int current_position = begin;
		while (node_i_can_pop.size() != 0)
		{
			int node = *node_i_can_pop.end().operator--();
			node_i_can_pop.pop_back();

			int node_position = id_to_position[node];
			
			// clear the reference of this node in the others
			for (auto next_node_id : centrini[node_position].go_before) {
				Centrino* next_node = &centrini[id_to_position[next_node_id]];
				auto to_erase = find(next_node->go_after.begin(), next_node->go_after.end(), node);
				next_node->go_after.erase(to_erase);

				if (next_node->go_after.size() == 0) {
					node_i_can_pop.push_back(next_node_id);
				}
			}

			swapp(current_position, id_to_position[node]);
			current_position++;
		}
		//return true if the solution has improove at least a bit
		return current_position != begin;
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
	int original_best = current_best;

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
	else {
		solution.greedy_ordering();
		solution.calculate_crossing();
	}

	if (current_best == original_best) {
		return 0;
	}

	{
		int start = rand() % solution.num_centrini;
		int end = start + rand() % 15;
		bool has_improve = solution.topological_order(start, end);
		if (has_improve) {
			output << solution;
		}
	}

	return 0;
}
