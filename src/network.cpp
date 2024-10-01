#pragma once
#include<vector>
#include"linalg.cpp"
#include"neuron.cpp"

class Network 
{
    public:
    int numinputs;

    Network(int inputs){
        numinputs = inputs;
    }

    void initialize_layer(int size){
        std::vector<Neuron> layer;
        for (int i = 0; i < size; i++)
            layer.push_back(Neuron());
        layers.push_back(layer);
    }

    void initialize_weights(){
        if (!layers.size()) return;
        Matrix<double> W1(numinputs, layers[0].size());
        W1.random();
        weights.push_back(W1);
        
        for (int i = 1; i < (int)layers.size() - 1; i++) {
            Matrix<double> W(layers[i].size(), layers[i + 1].size());
            W.random();
            weights.push_back(W);
        }
    };

    std::vector<double> get_output(){
        std::vector<double> output;
        int output_index = layers.size() - 1;

        for (int i = 0; i < (int)layers[output_index].size(); i++)
            output.push_back(layers[output_index][i].get());
        
        return output;
    }

    private:
    std::vector<std::vector<Neuron>> layers; 
    std::vector<Matrix<double>> weights;
};