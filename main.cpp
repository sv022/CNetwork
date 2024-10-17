#include<iostream>
#include"src/network.cpp"

using namespace std;


vector<double> input;
vector<double> expected;

int main(){
    int numinputs = 64;
    for (int i = 0; i < numinputs; i++){
        double value = randint(1, 100) / 100;
        input.push_back(value);
    }
    for (int i = 0; i < 10; i++){
        double value = randint(1, 100) / 100;
        expected.push_back(value);
    }
    Network N(numinputs);
    N.initialize_layer(16);
    N.initialize_layer(10);
    N.forward(input);
    vector<double> output = N.get_output();
    for (int i = 0; i < output.size(); i++){
        cout << output[i] << ' ';
    }
    cout << '\n';
}