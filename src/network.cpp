#pragma once
#include<vector>
#include"linalg.cpp"
#include"layer.cpp"

class Network 
{
    public:
    int numinputs;

    Network(int inputs){
        numinputs = inputs;
        Layer layer(inputs);
        layers.push_back(layer);
    }

    void initialize_layer(int size){
        Layer layer(size, layers.back().size);
        layers.push_back(layer);
    }

    std::vector<double> get_output(){
        std::vector<double> output;
        int output_index = layers.size() - 1;

        for (int i = 0; i < (int)layers[output_index].size; i++)
            output.push_back(layers[output_index].get(i));
        
        return output;
    }

    void forward(std::vector<double> input){
        int input_size = input.size();
        if (input_size != layers[0].size) return;
        for (int i = 0; i < input_size; i++){
            layers[0].set(i, input[i]);
        }
        for (int layer_num = 1; layer_num < (int)layers.size(); layer_num++){
            std::vector<double> layer = layers[layer_num].weights.dot(layers[layer_num - 1].get());
            for (int i = 0; i < (int)layer.size(); i++) {
                double value = sigmoid(layer[i] + layers[layer_num].biases[i]);
                layers[layer_num].set(i, value);
            }
        }
    }

    private:
    std::vector<Layer> layers; 
};