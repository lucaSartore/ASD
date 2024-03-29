#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <climits>
#include <queue>
using  namespace  std;

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


    int count_if_this_is_first_attacker(int k) {
        return count_nodes_in_range(distance_furthest_portal, distance_furthest_portal + k, -1);
    }

    int count_nodes_in_range(int min, int max, int coming_from) {
        if (distance_furthest_portal < min || distance_furthest_portal > max) {
            return 0;
        }
        int count = 1;
        for (auto& adj : adjacent) {
            if (adj.node->id == coming_from) {
                continue;
            }
            count += adj.node->count_nodes_in_range(min, max, id);
        }
        return count;
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

    void fill_fathers() {
        for (auto& node : nodes) {
            if ( node.adjacent.size() != 1 ) {
                root = node.id;
                break;
            }
        }

        nodes[root].fill_father(-1);
    }

    void fill_max_distance_down() {
        nodes[root].fill_max_distance_down();
    }

    void fill_max_distance() {
        nodes[root].fill_max_distance(0);
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

    g1.fill_fathers();
    g1.fill_max_distance_down();
    g1.fill_max_distance();

    input >> n;

    for (int i = 0 ; i < n; i++) {
        int max_v = 0;
        int k;
        input >> k;
        for (auto& node : g1.nodes) {
            max_v = max(max_v, node.count_if_this_is_first_attacker(k));
        }
        output << max_v << endl;
        //cout << max_v << " ";
    }

    input.close();
    output.close();
    return 0;
}
