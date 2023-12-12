#include <istream>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <tuple>
#include <unordered_set>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <cassert>
#include <limits>
using namespace std;

class Node;

inline int reach_options_size(Node* node);

const int INF = numeric_limits<int>::max();
const int IMPOSSIBLE = -1;

template<typename T>
ostream & operator<<(ostream & os, vector<T>& v);

class NodeRef{
public:
    int priority;
    Node* node;
    int index_reach_options_when_created;

    NodeRef(int _priority, Node* _node){
        priority = _priority;
        node = _node;
        index_reach_options_when_created = reach_options_size(node) - 1;
    }

    bool operator<(const NodeRef& other) const{
        return priority < other.priority;
    }
    bool operator>(const NodeRef& other) const{
        return priority > other.priority;
    }
    bool operator>=(const NodeRef& other) const{
        return priority >= other.priority;
    }
    bool operator<=(const NodeRef& other) const{
        return priority <= other.priority;
    }
};
class ReachOption;

ostream & operator<<(ostream & os, ReachOption& reach_option);

class ReachOption{
public:
    int n_hops;
    int base_cost;
    bool has_impostors;

    ReachOption(int _n_hops, int _base_cost, bool _has_impostors){
        n_hops = _n_hops;
        base_cost = _base_cost;
        has_impostors = _has_impostors;
    }

    // merge the two options, returns true if the options is ged
    bool merge(ReachOption* other){
        assert(n_hops == other->n_hops);

        if(other->base_cost > base_cost){
            return false;
        }
        if(other->base_cost == base_cost){
            has_impostors |= other->has_impostors;
            return true;
        }
        if(other->base_cost < base_cost){
            base_cost = other->base_cost;
            has_impostors = other->has_impostors;
            return true;
        }
    }

    int cost_at(int val_k){
        return n_hops*val_k + base_cost;
    }
};

class ReachOptions{
public:
    vector<ReachOption> options;

    ReachOptions(){
        options = vector<ReachOption>();
    }

    // returns true if the option was added
    bool add_option(ReachOption option){
        if(options.empty()){
            options.push_back(option);
            return true;
        }
        ReachOption* last_option = &options.back();

        if(last_option->n_hops > option.n_hops){
            return false;
        }

        if(last_option->n_hops == option.n_hops){
            last_option->merge(&option);
            return false;
        }

        // the new option has more hops than the last one
        // I insert it only if it is better
        if(last_option->base_cost < option.base_cost){
            return false;
        }

        // doto: in the case == i can avoid inseert it if there is no impostor

        options.push_back(option);
        return true;
    }


};

class Link{
public:
    int cost;
    Node* node;

    Link(Node* _node, int _cost ){
        node = _node;
        cost = _cost;
    }
};

class Node{
public:
    int value;
    vector<Link> adjacent_nodes;
    bool is_occupied;
    ReachOptions reach_options;

    Node(int _value){
        value = _value;
        adjacent_nodes = vector<Link>();
        is_occupied = false;
        reach_options = ReachOptions();
    }

    void insert_adjacent_node(Node* node_to_insert, int cost) {
        adjacent_nodes.emplace_back(node_to_insert,cost);
    }

};

inline int reach_options_size(Node* node){
    return node->reach_options.options.size();
}

class Graph{
public:
    vector<Node> nodes;
    int POS_BARBIE;
    int POS_ALGORITMIA;

    explicit Graph(int number_of_nodes, int _pos_barbie, int _pos_algoritmia){
        nodes = vector<Node>();
        nodes.reserve(number_of_nodes);
        for(int i=0; i<number_of_nodes; i++){
            nodes.emplace_back(i);
        }
        POS_BARBIE = _pos_barbie;
        POS_ALGORITMIA = _pos_algoritmia;
    }

    void insert_edge(int n1,int n2, int cost){
        nodes[n1].insert_adjacent_node(&nodes[n2],cost);
        nodes[n2].insert_adjacent_node(&nodes[n1],cost);
    }

    Node* get(int value){
        return &nodes[value];
    }

    // this function may couse problem if there are two paths with minimum ocst
    // and different number of hops... in theory it should return the one with the greater number of hops
    /// cost: o(n+m)
    vector<int> get_min_path(int from, int to){
        auto queue = priority_queue<NodeRef,vector<NodeRef>,greater<>>();
        vector<int> distances = vector<int>(nodes.size(),-1);

        queue.emplace(0,get(from));

        while(!queue.empty()){
            NodeRef node_ref = queue.top();
            queue.pop();
            Node* node = node_ref.node;
            int distance = node_ref.priority;
            if(distances[node->value] != -1){
                continue;
            }
            distances[node->value] = distance;

            for(auto adjacent_node: node->adjacent_nodes){
                if(distances[adjacent_node.node->value] != -1){
                    continue;
                }
                queue.emplace(distance+adjacent_node.cost,adjacent_node.node);
            }
        }
        int min_distance = distances[to];
        assert(min_distance != -1);

        vector<int> path = vector<int>();

        int current_node = to;
        path.push_back(current_node);
        while(current_node!=from){
            for(auto adj: get(current_node)->adjacent_nodes){
                if(min_distance == distances[adj.node->value] + adj.cost){
                    current_node = adj.node->value;
                    path.push_back(current_node);
                    min_distance -= adj.cost;
                    break;
                }
            }

        }
        std::reverse(path.begin(), path.end());

        return path;
    }

    void propagate_reach_options(int from_id, int number_of_hops){

        Node* from = get(from_id);

        auto queue = priority_queue<NodeRef,vector<NodeRef>,greater<>>();

        from->reach_options.add_option(ReachOption(0,0,false));

        queue.emplace(0,from);

        while(!queue.empty()){
            NodeRef node_ref = queue.top();
            queue.pop();

            int n_hops = node_ref.priority;
            Node* node = node_ref.node;

            //cout << "Node " << node->value << " options: " << node->reach_options.options << endl;

            if(n_hops > number_of_hops){
                break;
            }

            ReachOption* option_wen_pushed = &node->reach_options.options[node_ref.index_reach_options_when_created];
            int this_node_base_cost = option_wen_pushed->base_cost;
            bool previous_path_has_impostor = option_wen_pushed->has_impostors;
            for(auto& adjacent_node: node->adjacent_nodes){
                bool has_impostor = previous_path_has_impostor || adjacent_node.node->is_occupied;
                //cout << "From " << node->value << " to " << adjacent_node.node->value << " has impostor: " << has_impostor << endl;

                ReachOption option_for_adj_node = ReachOption(n_hops+1,  this_node_base_cost + adjacent_node.cost, has_impostor);

                if(adjacent_node.node->reach_options.add_option(option_for_adj_node)){
                    queue.emplace(n_hops+1,adjacent_node.node);
                }
            }
        }

    }
};



class Interval{
public:
    int start;
    int end;
    bool has_impostor;
    Interval(int _start, int _end, bool _has_impostor){
        start = _start;
        end = _end;
        has_impostor = _has_impostor;
    }
};


ostream & operator<< (ostream& os, Interval& interval);

// point of intersection of two lines
float intersection_point(ReachOption* line_1, ReachOption* line_2){
    int m1 = line_1->n_hops;
    int q1 = line_1->base_cost;
    int m2 = line_2->n_hops;
    int q2 = line_2->base_cost;

    float numerator = (float)(q2-q1);
    float denominator = (float)(m1-m2);

    return numerator/denominator;
}

// return the K for witch option start to have a lower or equal, cost than the previous option
int low_bound(ReachOption* option, ReachOption* previous_option){
    if(previous_option == nullptr){
        return 0;
    }

    assert(option->n_hops < previous_option->n_hops);
    assert(option->base_cost >= previous_option->base_cost);

    float x = intersection_point(option,previous_option);

    int x_int = floor(x);

    return x_int;
}

// return the K for witch option stop having a lower or equal, cost than the next option
int high_bound(ReachOption* option, ReachOption* next_option){
    if(next_option == nullptr){
        return INF;
    }

    assert(option->n_hops > next_option->n_hops);
    assert(option->base_cost <= next_option->base_cost);

    float x = intersection_point(option,next_option);

    int x_int = ceil(x);

    // condition for lack of precision floating point division
    if(option->cost_at(x_int) == next_option->cost_at(x_int)){
        return x_int;
    }
    if(option->cost_at(x_int+1) == next_option->cost_at(x_int+1)){
        return x_int+1;
    }
    if(option->cost_at(x_int-1) == next_option->cost_at(x_int-1)){
        return x_int-1;
    }

    return x_int;
}

ReachOption* next_option(vector<ReachOption>& options, ReachOption* value){
    ReachOption* last = &options.back();
    int original_hops = value->n_hops;
    while (true){
        if(value == last){
            return nullptr;
        }
        value++;
        if(value->n_hops != original_hops) {
            return value;
        }
    }
}

ReachOption* prev_option(vector<ReachOption>& options, ReachOption* value){
    ReachOption* first = &options.front();
    int original_hops = value->n_hops;
    while (true){
        if(value == first){
            return nullptr;
        }
        value--;
        if(value->n_hops != original_hops) {
            return value;
        }
    }
}

vector<Interval> get_intervals(ReachOptions& reach_options){
    vector<Interval> intervals = vector<Interval>();

    vector<ReachOption> options = reach_options.options;

    reverse(options.begin(),options.end());

    // now the options are in order from the one with
    // the lowest base-cost to the one with the highest base-cost
    // and to the one with highest number of hops to the one with lowest number of hops

    //cout << "options: " << options << endl;

    for(auto& option: options){

        ReachOption* next_option_ptr = next_option(options,&option);
        ReachOption* prev_option_ptr = prev_option(options,&option);

        int start_interval = low_bound(&option,prev_option_ptr);
        int end_interval = high_bound(&option,next_option_ptr);

        intervals.emplace_back(start_interval,end_interval,option.has_impostors);
    }

    return intervals;
}

typedef enum KValueKind{AddImpostor,RemoveImpostor,TestableOption};

class KValueToTest{
    int order_value;
public:
    int k_value;
    KValueKind kind;

    KValueToTest(int _k_value, KValueKind _kind){
        k_value = _k_value;
        kind = _kind;
        if(k_value == INF){
            order_value = INF;
        }else{
            // this make sure that if the time is the same, the impostor is always added
            // first and removed last
            order_value = k_value*3;
            if(kind == RemoveImpostor) {
                order_value += 2;
            }else if (kind == TestableOption){
                order_value += 1;
            }
        }
    }

    bool operator<(const KValueToTest& other) const{
        return order_value < other.order_value;
    }
    bool operator>(const KValueToTest& other) const{
        return order_value > other.order_value;
    }
    bool operator>=(const KValueToTest& other) const{
        return order_value >= other.order_value;
    }
    bool operator<=(const KValueToTest& other) const{
        return order_value <= other.order_value;
    }
};


int get_best_k(vector<Interval>& intervals){
    vector<KValueToTest> k_value_to_test = vector<KValueToTest>();

    for(auto& interval: intervals){
        if(interval.has_impostor){
            k_value_to_test.emplace_back(interval.start,AddImpostor);
            k_value_to_test.emplace_back(interval.end,RemoveImpostor);
        }else{
            k_value_to_test.emplace_back(interval.start,TestableOption);
            if(interval.end == interval.start){
                continue;
            }
            k_value_to_test.emplace_back(interval.end,TestableOption);
            if(interval.start+1 == interval.end){
                continue;
            }
            k_value_to_test.emplace_back(interval.start+1,TestableOption);
            if(interval.start+2 == interval.end){
                continue;
            }
            k_value_to_test.emplace_back(interval.end-1,TestableOption);
        }
    }

    sort(k_value_to_test.begin(),k_value_to_test.end());

    int n_impostor = 0;
    int best_k = IMPOSSIBLE;

    for(auto item: k_value_to_test){
        if(item.kind == AddImpostor){
            n_impostor++;
        }else if(item.kind == RemoveImpostor){
            n_impostor--;
        }else if(n_impostor == 0){
            best_k = item.k_value;
        }
    }

    return best_k;
}

int main(){
    int n_nodes;
    int n_edges;
    int n_occupied_nodes;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n_nodes >> n_edges;

    const int POS_BARBIE = 0;
    const int POS_ALGORITMIA = n_nodes-1;

    Graph graph(n_nodes,POS_BARBIE,POS_ALGORITMIA);

    for(int i=0; i<n_edges; i++){
        int n1,n2,cost;
        input >> n1 >> n2 >> cost;
        graph.insert_edge(n1,n2,cost);
    }

    input >> n_occupied_nodes;

    for(int i=0; i<n_occupied_nodes; i++){
        int occupied_node;
        input >> occupied_node;
        graph.nodes[occupied_node].is_occupied = true;
    }

    int max_num_hops = graph.get_min_path(POS_BARBIE,POS_ALGORITMIA).size()-1;

    graph.propagate_reach_options(POS_BARBIE,max_num_hops);

    auto intervals = get_intervals(graph.nodes[POS_ALGORITMIA].reach_options);

    //cout << intervals;

    int best_k = get_best_k(intervals);

    //cout << "The best K is: " << best_k << endl;

    if(best_k == IMPOSSIBLE){
        output << -2 << endl;
    }else if(best_k == INF){
        output << -1 << endl;
    }else{
        output << best_k << endl;
    }

    output.close();
    input.close();
}


ostream & operator<<(ostream & os, ReachOption& reach_option){
    os << "n_hops: " << reach_option.n_hops << " base_cost: " << reach_option.base_cost << " has_impostors: " << reach_option.has_impostors;
    return os;
}

template<typename T>
ostream & operator<<(ostream & os, vector<T>& v){
    os << "[" << endl;
    int c = 0;
    for(auto node: v){
        os << "\t" << c <<": " << node << "\n";
        c++;
    }
    os << "]\n";
    return os;
}



ostream & operator<< (ostream& os, Interval& interval){

    os << "[" << interval.start << ", ";
    if(interval.end == INF){
        os << "+inf";
    }else{
        os << interval.end;
    }
    os << "] has impostor: " << interval.has_impostor;
    return os;
}