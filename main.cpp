#include<iostream>
#include"src/network.cpp"

using namespace std;


std::vector<std::vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
std::vector<std::vector<double>> expected = {{0}, {1}, {1}, {0}};

int main(){
    int numinputs = 2;

    Network N(numinputs);
    N.initialize_layer(2);
    N.initialize_layer(1);

    N.train(inputs, expected, 10000);

    for (int i = 0; i < (int)inputs.size(); i++) {
        N.forward(inputs[i]);
        vector<double> output = N.get_output();
        cout << "Input: " << inputs[i][0] << ", " << inputs[i][0] << " -> Output: " << output[0] << '\n';
    }
    cout << '\n';
}