#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <climits>
#include <queue>
#include <set>
#include <cassert>
using  namespace  std;

int max_k;

class ReachOptions {
public:

    map<int, int, greater<int>> distance_to_number_of_soldier;

    ReachOptions() {
        distance_to_number_of_soldier = map<int, int, greater<int>>();
        this->add_reach_option(0, 1);
    }

    int get_number_of_soldier_with_cost_lower_or_equal_to(int cost) {
        auto x = distance_to_number_of_soldier.lower_bound(cost);
        if (x == distance_to_number_of_soldier.end()) {
            return 0;
        }
        return x->second;
    }

     void increase_distance_by(int distance_difference) {

        map<int, int, greater<int>> new_distance_to_number_of_soldier = map<int, int, greater<int>>() ;

        for (auto& item : distance_to_number_of_soldier) {
            new_distance_to_number_of_soldier[item.first + distance_difference] = item.second;
        }

        distance_to_number_of_soldier = new_distance_to_number_of_soldier;
    }

    void add_reach_option(int distance, int n_of_soldier) {
        distance_to_number_of_soldier[distance] = n_of_soldier;
    }

    static ReachOptions merge(vector<ReachOptions>& to_merge) {

        set<int> distances_to_consider = set<int>();

        for (auto& option : to_merge) {
            for (auto& reach_option : option.distance_to_number_of_soldier) {
                int distance = reach_option.first;
                if (distance > max_k) {
                    continue;
                }
                distances_to_consider.emplace(distance);
            }
        }

        ReachOptions to_return = ReachOptions();

        for (int distance : distances_to_consider) {

            int total_soldiers = 0;

            for (auto& options : to_merge) {
                total_soldiers += options.get_number_of_soldier_with_cost_lower_or_equal_to(distance);
            }
            
            to_return.add_reach_option(distance, total_soldiers);
        }
        return to_return;
    }

};


class FirstStageNode;

class Link{
public:
    FirstStageNode* node;
    int distance;
};


class FirstStageNode{
public:
    
    int id;
    vector<Link> adjacent;
    int father_id;
    int distance_furthest_portal_down;
    int distance_furthest_portal;

    void insert_adjacent(FirstStageNode* node, int distance){
        adjacent.push_back(Link{node, distance});
    }

    explicit FirstStageNode(int _id){
        id = _id;
        adjacent = vector<Link>();
    }

    
    void fill_father(int my_father) {
        father_id = my_father;
        for (auto& child : adjacent) {
            if (child.node->id == my_father) {
                continue;
            }
            child.node->fill_father(id);
        }
    }

    void fill_max_distance_down() {
        int max_distance = 0;
        for (auto& child : adjacent) {
            if (child.node->id == father_id) {
                continue;
            }
            child.node->fill_max_distance_down();
            int distance_child = child.node->distance_furthest_portal_down + child.distance;
            max_distance = max(max_distance, distance_child);
        }
        distance_furthest_portal_down = max_distance;
    }

    void fill_max_distance(int max_distance_up) {

        int max_distance = max_distance_up;
        int second_max_distance = 0;


        for (auto& child: adjacent) {
            if (child.node->id == father_id) {
                continue;
            }
            int new_distance = child.distance + child.node->distance_furthest_portal_down;

            if (new_distance >= max_distance) {
                second_max_distance = max_distance;
                max_distance = new_distance;
            }
            else if (new_distance > second_max_distance) {
                second_max_distance = new_distance;
            }
        }

        distance_furthest_portal = max_distance;

        for (auto& child : adjacent) {
            
            if (child.node->id == father_id) {
                continue;
            }
            int new_distance = child.distance + child.node->distance_furthest_portal_down;

            if (new_distance == max_distance) {
                child.node->fill_max_distance(second_max_distance + child.distance);
            }
            else {
                child.node->fill_max_distance(max_distance + child.distance);
            }
        }

    }

};

class FirstStageGraph {
public:
    vector<FirstStageNode> nodes;
    int root;

    explicit FirstStageGraph(int n_nodes) {
        nodes = vector<FirstStageNode>();
        nodes.reserve(n_nodes);
        for (int i = 0; i < n_nodes; i++) {
            nodes.emplace_back(i);
        }
    }
    void insert_link(int n1, int n2, int cost) {
        nodes[n1].insert_adjacent(&nodes[n2], cost);
        nodes[n2].insert_adjacent(&nodes[n1], cost);
    }

    class InQueueObject {
    public:
        int priority;
        FirstStageNode* node;
        bool operator<(InQueueObject const& other) const {
            return priority > other.priority;
        }
    };

    void fill_fathers(int father = -1) {
        if (father == -1) {
            for (auto& node : nodes) {
                if ( node.adjacent.size() != 1 ) {
                    root = node.id;
                    break;
                }
            }
        }
        else {
            root = father;
        }

        nodes[root].fill_father(-1);
    }

    void fill_fathers_from_closest_city() {
        int closest_city = 0;
        int distance_furthest_portal_min = INT_MAX;

        for (auto& node : nodes) {
            if (node.distance_furthest_portal < distance_furthest_portal_min) {
                distance_furthest_portal_min = node.distance_furthest_portal;
                closest_city = node.id;
            }
        }
        nodes[closest_city].father_id = -1;
        fill_fathers(closest_city);
    }

    void fill_max_distance_down() {
        nodes[root].fill_max_distance_down();
    }

    void fill_max_distance() {
        nodes[root].fill_max_distance(0);
    }


    void optimize_memory() {
        for (auto& n : nodes) {
            n.adjacent.shrink_to_fit();
        }
    }

    void forget_everything() {
        nodes = vector<FirstStageNode>();
    }
};


class SecondStageNode{
public:
    
    vector<SecondStageNode*> adjacent;
    ReachOptions reach_options;
    int distance_furthest_portal;

    void insert_adjacent(SecondStageNode* node){
        adjacent.push_back(node);
    }

    explicit SecondStageNode(int _distance_furthest_portal){
        distance_furthest_portal = _distance_furthest_portal;
        adjacent = vector<SecondStageNode*>();
        reach_options = ReachOptions();
    }

    int count_if_this_is_first_attacker(int k) {
        return reach_options.get_number_of_soldier_with_cost_lower_or_equal_to(k);
    }


    void fill_reach_options() {

        vector<ReachOptions> v = vector<ReachOptions>();

        v.push_back(ReachOptions());

        for (auto& child : adjacent) {
            child->fill_reach_options();
            auto child_reach_options = child->reach_options;
            int increasing_distance = child->distance_furthest_portal - distance_furthest_portal;
            child_reach_options.increase_distance_by(increasing_distance);
            v.push_back(std::move(child_reach_options));
        }

        reach_options = ReachOptions::merge(v);
    }
};

class SecondStageTree {
public:
    vector<SecondStageNode> nodes;
    int root;

    SecondStageTree(FirstStageGraph & g1) {
        root = g1.root;
        nodes = vector<SecondStageNode>();
        nodes.reserve(g1.nodes.size());
        
        for (auto& node : g1.nodes) {
            nodes.push_back(SecondStageNode(node.distance_furthest_portal));
        }
        
        for (auto& node : g1.nodes) {
            for (auto& child : node.adjacent) {
                if (child.node->id == node.father_id) {
                    continue;
                }
                insert_link(node.id, child.node->id);
            }
            nodes[node.id].adjacent.shrink_to_fit();
            node.adjacent = vector<Link>();
        }
    }
    void insert_link(int father, int chil) {
        nodes[father].insert_adjacent(&nodes[chil]);
    }

    void fill_reach_options() {
        nodes[root].fill_reach_options();
    }

    void optimize_memory() {
        for (auto& n : nodes) {
            n.adjacent.shrink_to_fit();
        }
    }
};


int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");
    int n;

    input >> n;


    auto g1 = FirstStageGraph(n);

    for(int i=0; i<n-1; i++){
        int p,a,w;
        input >> p >> a >> w;
        g1.insert_link(p,a,w);
    }

    g1.optimize_memory();

    input >> n;

    vector<int> k_vec = vector<int>();
    k_vec.reserve(n);

    max_k = 0;

    for (int i = 0 ; i < n; i++) {
        int max_v = 0;
        int k;
        input >> k;
        k_vec.push_back(k);
        max_k = max(max_k, k);
    }

    g1.fill_fathers();
    g1.fill_max_distance_down();
    g1.fill_max_distance();
    g1.fill_fathers_from_closest_city();
    //return 0;
    auto g2 = SecondStageTree(g1);
    g1.forget_everything();
    g2.fill_reach_options();
    g2.optimize_memory();

    for (int k : k_vec) {
        int max_v = 0;
        for (auto& node : g2.nodes) {
            max_v = max(max_v, node.count_if_this_is_first_attacker(k));
        }
        output << max_v << endl;
        cout << max_v << " ";
    }

    input.close();
    output.close();
    return 0;
}
