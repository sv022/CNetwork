#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<stdexcept>
#include"file.cpp"
#include"neuron.cpp"


int randint(int a, int b){
    return a + (std::rand() % (b - a));
}

bool contains(std::vector<int> a, int value){
	for (unsigned i = 0; i < a.size(); i++) {
		if (a[i] == value) return true;
	}
	return false;
}


class Network {

private:
	std::vector<Layer> layers;
	double error;
	double averageError;
	static double smoothingFactor;
	int getMaxActivationIndex(std::vector<double> layerOutputs);
	void feedForward(const std::vector<double> &inputs);
	void backProp(const std::vector<double> &targets);
	std::vector<double> getOutput() const;
	std::vector<Layer> getLayers() const { return layers; }
	

public:
	Network(int layer_count, std::vector<int> layers_size);
	double getRecentAverageError(void) const { return averageError; }
	void train(std::string filePath, int epochs);
    int predict(std::vector<double> input);

};


double Network::smoothingFactor = 100;

Network::Network(int layer_count, std::vector<int> layers_size) {
	if (layer_count != layers_size.size()) throw std::runtime_error("Invalid network layout.");
	layers.reserve(layer_count);
	error = 0;
	averageError = 0;
	for (unsigned l = 0; l < layers_size.size(); l++) {

		layers.push_back(Layer());

		// Проверка на выходной слой
		unsigned outputAmt = (l == layer_count - 1 ? 0 : layers_size[l + 1]);

		layers.back().reserve(layers_size[l]);
		for (unsigned n = 0; n <= layers_size[l]; n++)
			layers.back().push_back(Neuron(outputAmt, n));

		layers.back().back().setOutput(1);
	}
}

// Получить значения выходного слоя
std::vector<double> Network::getOutput() const {
    std::vector<double> results;

	for (unsigned n = 0; n < layers.back().size() - 1; n++)
		results.push_back(layers.back()[n].getOutput());

    return results;
}

void Network::backProp(const std::vector<double> &targets) {
	Layer &outputLayer = layers.back();
	error = 0.0;

	// ошибка на выходном слое
	for (unsigned n = 0; n < outputLayer.size() - 1; n++) {
		double delta = targets[n] - outputLayer[n].getOutput();
		error += delta * delta;
	}
	error /= outputLayer.size() - 1;
	error = sqrt(error);

	// averageError += (averageError * smoothingFactor + error) / (smoothingFactor + 1);

	// Градиент для выходного слоя
	for (unsigned n = 0; n < outputLayer.size() - 1; n++) {
		outputLayer[n].calculateOutputGradients(targets[n]);
	}
	
	// Градиенты для скрытых слоев
	for (unsigned layerNum = layers.size() - 2; layerNum > 0; --layerNum) {
		for (unsigned n = 0; n < layers[layerNum].size(); n++) {
			layers[layerNum][n].calculateHiddenGradients(layers[layerNum + 1]);
		}
	}

	// Обновляем веса
	for (unsigned layerNum = layers.size() - 1; layerNum > 0; layerNum--) {
		for (unsigned n = 0; n < layers[layerNum].size() - 1; n++) {
			layers[layerNum][n].updateWeights(layers[layerNum - 1]);
		}
	}
}

void Network::feedForward(const std::vector<double> &inputVals) {
	for (unsigned i = 0; i < inputVals.size(); i++) {
		layers[0][i].setOutput(inputVals[i]);
	}

	for (unsigned layerNum = 1; layerNum < layers.size(); layerNum++) {
		for (unsigned n = 0; n < layers[layerNum].size() - 1; n++)
			layers[layerNum][n].feedForward(layers[layerNum - 1]);
	}
}


void Network::train(std::string dataPath, int epochs) {
	File file(dataPath);

    int iteration = 0;
	int epoch = 1;
    bool debug = true;

    if (debug) std::cout << "epoch: " << epoch << std::endl;
	// std::vector<int> usedInputs;

	while (epoch <= epochs) {
		// int index = (std::rand() % static_cast<int>(file.getDataSize() + 1));
		int index = randint(0, file.getDataSize());
		
		// while (contains(usedInputs, index)) {
		// 	index = randint(0, file.getDataSize());
		// 	if (debug) std::cout << index << ' ';
		// }
		

		std::vector<double> inputs = file.getInputs(index);
		std::vector<double> targets = file.getTargets(index);
		if (targets.size() == 0 || inputs.size() == 0) continue;

		feedForward(inputs);
		backProp(targets);

        if (debug) std::cout << "Targets: ";
        if (debug) {
            for (unsigned i = 0; i < targets.size(); i++)
                std::cout << targets[i] << " ";
        }

		std::vector<double> results = getOutput();

		if (debug) std::cout << "Results: ";
        if (debug) {
            for (unsigned i = 0; i < results.size(); i++)
                std::cout << results[i] << " ";
			std::cout << '\n';
        }

        for (int i = 0; i < (int)results.size(); i++){
            error += (targets[i] - results[i]) * (targets[i] - results[i]) ;
        }

		// usedInputs.push_back(index);
		iteration++;

		if (iteration == file.getMaxIterations()) {
            if (debug) std::cout << error / iteration << std::endl;
			iteration = 0;
			// usedInputs.clear();
            epoch++;
    		if (debug && epoch < epochs) std::cout << "epoch: " << epoch << std::endl;
		}
	}
}

// Индекс нейрона с наибольшим выходным значением
int Network::getMaxActivationIndex(std::vector<double> layerOutputs){
        int maxIndex = -1;
        double maxVal = -1000000;
        for (unsigned i = 0; i < layerOutputs.size(); i++){
            if (layerOutputs[i] > maxVal) {
                maxVal = layerOutputs[i];
                maxIndex = i;
            }
        }
        if (maxIndex == -1) throw std::runtime_error("Incorrent output values.");
        return maxIndex;
    } 

int Network::predict(std::vector<double> input){
    feedForward(input);
    std::vector<double> output = getOutput();
    int prediction = getMaxActivationIndex(output);
    return prediction;
}