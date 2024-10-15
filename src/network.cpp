#pragma once
#include<vector>
#include"linalg.cpp"
#include"layer.cpp"
#include <fstream>

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

    double Cost(std::vector<double> expected){
        std::vector<double> output_layer = get_output();
        if (expected.size() != output_layer.size()) throw std::invalid_argument("Invalid output size");

        double cost = 0;

        for (int i = 0; i < (int)output_layer.size(); i++) {
            cost += ((output_layer[i] - expected[i]) * (output_layer[i] - expected[i]));
        }
        return cost;
    }

    void save_weights(std::string file_path) {
        std::ofstream file;
        file.open(file_path, std::ios::binary);
        for (Layer layer : std::vector<Layer>(layers.begin() + 1, layers.end())) {
            int layer_size = layer.size;
            file.write((char*)&layer_size, sizeof(layer_size));

            for (int i = 0; i < layer.weights.shape.first; i++) {
                for (int j = 0; j < layer.weights.shape.second; j++) {
                    double weight = layer.weights[i][j];
                    file.write((char*)&weight, sizeof(weight));
                }
            }

            for (int i = 0; i < (int)layer.size; i++) {
                double bias = layer.biases[i];
                file.write((char*)&bias, sizeof(bias));
            }
        }
    }

    void load_weights_biases(std::string filepath) {
    std::ifstream infile(filepath, std::ios::binary);

    if (!infile.is_open()) {
        throw std::runtime_error("Unable to open file for reading");
    }

    for (int layer = 1; layer < layers.size(); layer++) {
        int layer_size = 0;
        infile.read((char*)&layer_size, sizeof(layer_size));

        for (int i = 0; i < layers[layer].weights.shape.first; i++) {
            for (int j = 0; j < layers[layer].weights.shape.second; j++) {
                double weight = 0;
                infile.read((char*)&weight, sizeof(weight));
                layers[layer].weights[i][j] = weight;
            }
        }

        for (int i = 0; i < layers[layer].size; i++) {
            double bias = 0;
            infile.read((char*)&bias, sizeof(bias));
            layers[layer].biases[i] = bias;
        }
    }

    infile.close();
}

    private:
    std::vector<Layer> layers; 
};