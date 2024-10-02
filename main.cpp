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
    
    Matrix<double> M1(3, 3);
    Matrix<double> M2(3, 1);
    M1.random();
    M2.random();
    Matrix<double> M = M1.dot(M2);
    std::cout << M[0][0] << '\n';
    std::cout << M[1][0] << '\n';
    std::cout << M[2][0] << '\n';
}