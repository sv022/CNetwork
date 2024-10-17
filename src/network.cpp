#pragma once
#include<vector>
#include"linalg.cpp"
#include"layer.cpp"
#include <fstream>
#include <stdexcept>

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

        for (int layer = 1; layer < (int)layers.size(); layer++) {
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
            infile.close();
        }
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

    void backpropagate(const std::vector<double>& expected_output){
        // Ошибки на выходном слое
        Layer& output_layer = layers.back();
        std::vector<double> output_errors(output_layer.size);

        for (int i = 0; i < output_layer.size; i++) {
            double output_value = output_layer.get(i);
            output_errors[i] = (expected_output[i] - output_value) * sigmoid_derivative(output_value);
        }

        // Ошибки для скрытых слоев
        for (int layer_num = layers.size() - 2; layer_num >= 0; layer_num--){
            Layer& current_layer = layers[layer_num];
            Layer& next_layer = layers[layer_num + 1];

            std::vector<double> layer_errors(current_layer.size);
            for (int i = 0; i < current_layer.size; i++) {
                double error = 0.0;
                for (int j = 0; j < next_layer.size; j++) {
                    error += next_layer.weights[j][i] * output_errors[j];
                }
                layer_errors[i] = error * sigmoid_derivative(current_layer.get(i));
            }

            // Обновление весов и смещений для следующего слоя
            for (int i = 0; i < next_layer.size; i++) {
                for (int j = 0; j < current_layer.size; j++) {
                    next_layer.weights[i][j] += learning_rate * output_errors[i] * current_layer.get(j);
                }
                next_layer.biases[i] += learning_rate * output_errors[i];
            }

            output_errors = layer_errors; // Переносим ошибки для следующего шага
        }
    }

    void train(const std::vector<std::vector<double>>& inputs, const std::vector<std::vector<double>>& outputs, int epochs){
        for (int epoch = 0; epoch < epochs; epoch++){
            for (size_t i = 0; i < inputs.size(); i++){
                forward(inputs[i]);
                backpropagate(outputs[i]);
            }
        }
    }

    private:
    double learning_rate = 0.1;
    std::vector<Layer> layers;
    
    double cost(std::vector<double> expected){
        std::vector<double> output_layer = get_output();
        if (expected.size() != output_layer.size()) throw std::invalid_argument("Invalid output size");

        double cost = 0;

        for (int i = 0; i < (int)output_layer.size(); i++) {
            cost += ((output_layer[i] - expected[i]) * (output_layer[i] - expected[i]));
        }
        return cost;
    }
};