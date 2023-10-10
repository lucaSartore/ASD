#include <iostream>
#include <vector>

using namespace std;
class Cestino{
public:
    vector<int> stack;


    Cestino(int n, bool filled=false){
        stack = vector<int>();
        stack.reserve(n);
        if(filled){
            for(int i=0; i<n; i++){
                stack.push_back(i);
            }
        }
    }

    void insert(int val){
        stack.push_back(val);
    }

    int get(){
        try{
            int a = stack.back();
            stack.pop_back();
            return a;
        }catch (...){
            throw "empty basket";
        }
    }
};

class Camera{
public:
    vector<Cestino> cestini;

    Camera(int num_cestini, int dim_cestino){
        cestini = vector<Cestino>();
        cestini.reserve(num_cestini);

        Cestino cestino = Cestino(dim_cestino, true);
        cestini.push_back(std::move(cestino));

        for(int i=1; i<num_cestini; i++){
            cestino = Cestino(dim_cestino);
            cestini.push_back(cestino);
        }
    }

    int test(int id_cestino, int index_oggetto){
        try{
            return cestini.at(id_cestino).stack.at(index_oggetto);
        }catch(...){
            return -1;
        }
    }

    void muovi(int from, int to){
        cestini[to].insert(cestini[from].get());
    }
};

Camera camera(0,0);

void inizia(int N, int M){
    camera = Camera(M,N);
}

void sposta(int a, int b) {
    camera.muovi(a,b);
}

int controlla(int a, int i) {
    return camera.test(a,i);
}

//int main(){};