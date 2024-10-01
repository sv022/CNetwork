#pragma once
#include<vector>
#include"neuron.cpp"
#include"linalg.cpp"


class Layer
{
    public:
    int size;
    Matrix<double> weights;

    Layer(int n){
        size = n;
        for (int i = 0; i < n; i++) {
            neurons.push_back(Neuron());
        }
    }

    Layer(int n, int prev_size) {
        size = n;
        for (int i = 0; i < n; i++) {
            neurons.push_back(Neuron());
        }
        Matrix<double> new_weights(prev_size, n);
        new_weights.random();
        weights = new_weights;
    }

    double get(int i){
        return neurons[i].get();
    }

    void set(int i, double value){
        neurons[i].set(value);
    }

    private:
    std::vector<Neuron> neurons;
};