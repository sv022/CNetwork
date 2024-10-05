#pragma once
#include<vector>
#include"neuron.cpp"
#include"linalg.cpp"


class Layer
{
    public:
    int size;
    Matrix<double> weights;
    std::vector<double> biases;

    // input layer
    Layer(int n){
        size = n;
        for (int i = 0; i < n; i++) {
            neurons.push_back(Neuron());
        }
    }

    // hidden / output layers
    Layer(int n, int prev_size) {
        size = n;
        for (int i = 0; i < n; i++) {
            neurons.push_back(Neuron());
        }
        Matrix<double> new_weights(n, prev_size);
        new_weights.random();
        weights = new_weights;

        for (int i = 0; i < n; i++)
            biases.push_back((double)randint(1, 100) / 100);
    }

    double get(int i){
        return neurons[i].get();
    }

    std::vector<double> get(){
        std::vector<double> result;
        for (int i = 0; i < size; i++)
            result.push_back(neurons[i].get());
        return result;
    }

    void set(int i, double value){
        neurons[i].set(value);
    }

    private:
    std::vector<Neuron> neurons;
};