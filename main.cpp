#include<iostream>
#include"src/network.cpp"

using namespace std;


vector<double> input;

int main(){
    int numinputs = 64;
    for (int i = 0; i < numinputs; i++){
        double value = randint(0, 1);
        input.push_back(value);
    }
    
    Network N(64);
    N.initialize_layer(16);
    N.initialize_layer(10);
    N.initialize_weights();
}