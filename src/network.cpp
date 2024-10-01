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
    }

    void initialize_layer(int size){
        if (!layers.size()) {
            Layer layer(size);
            layers.push_back(layer);
        }
        else {
            Layer layer(size, layers.back().size);
            layers.push_back(layer);
        } 
    }

    std::vector<double> get_output(){
        std::vector<double> output;
        int output_index = layers.size() - 1;

        for (int i = 0; i < (int)layers[output_index].size; i++)
            output.push_back(layers[output_index].get(i));
        
        return output;
    }

    private:
    std::vector<Layer> layers; 
};