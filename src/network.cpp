#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<stdexcept>
#include"file.cpp"
#include"neuron.cpp"


class Network {

private:
	std::vector<Layer> layers;
	double error;
	double averageError;
	static double smoothingFactor;

    int getMaxActivationIndex(std::vector<double> layerOutputs){
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

public:
	Network(const std::vector<unsigned> &layout);
	void feedForward(const std::vector<double> &inputs);
	void backProp(const std::vector<double> &targets);
	std::vector<double> getOutput() const;
	double getRecentAverageError(void) const { return averageError; }
	std::vector<Layer> getLayers() const { return layers; }
	void train(std::string filePath, int epochs);
    int predict(std::vector<double> input);

};


double Network::smoothingFactor = 100;


std::vector<double> Network::getOutput() const {
    std::vector<double> results;

	for (unsigned n = 0; n < layers.back().size() - 1; n++)
		results.push_back(layers.back()[n].getOutput());

    return results;
}

void Network::backProp(const std::vector<double> &targets) {
	Layer &outputLayer = layers.back();
	error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; n++) {
		double delta = targets[n] - outputLayer[n].getOutput();
		error += delta * delta;
	}
	error /= outputLayer.size() - 1;
	error = sqrt(error);

	averageError += (averageError * smoothingFactor + error) / (smoothingFactor + 1);

	for (unsigned n = 0; n < outputLayer.size() - 1; n++) {
		outputLayer[n].calculateOutputGradients(targets[n]);
	}

	for (unsigned layerNum = layers.size() - 2; layerNum > 0; --layerNum) {
		for (unsigned n = 0; n < layers[layerNum].size(); n++) {
			layers[layerNum][n].calculateHiddenGradients(layers[layerNum + 1]);
		}
	}

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

Network::Network(const std::vector<unsigned> &layout) {
	layers.reserve(layout.size());
	error = 0;
	averageError = 0;
	for (unsigned l = 0; l < layout.size(); l++) {
		layers.push_back(Layer());
		unsigned outputAmt = l == layout.size() - 1 ? 0 : layout[l + 1];

		layers.back().reserve(layout[l]);
		for (unsigned n = 0; n <= layout[l]; n++)
			layers.back().push_back(Neuron(outputAmt, n));

		layers.back().back().setOutput(1);
	}
}

void Network::train(std::string filePath, int epochs) {
	File file("data.txt");

    int iteration = 0;
	int epoch = 0;
    bool debug = false;

    if (debug) std::cout << "epoch: " << epoch << std::endl;
	while (epoch < epochs) {

		std::vector<double> inputs = file.getInputs(iteration % file.getDataSize());
		feedForward(inputs);

		std::vector<double> targets = file.getTargets(iteration % file.getDataSize());
		backProp(targets);

        if (debug) std::cout << "Targets: " << std::flush;

        if (debug) {
            for (unsigned i = 0; i < targets.size(); i++)
                std::cout << targets[i] << " " << std::flush;
        }

		if (debug) std::cout << std::endl << "Results: " << std::flush;

		std::vector<double> results = getOutput();

        if (debug) {
            for (unsigned i = 0; i < results.size(); i++)
                std::cout << results[i] << " " << std::flush;
        }

        for (int i = 0; i < (int)results.size(); i++){
            error += (targets[i] - results[i]) * (targets[i] - results[i]) ;
        }

		iteration++;

		if (iteration == file.getMaxIterations()) {
            if (debug) std::cout << '\n' << error / iteration << std::endl;
			// std::string choice;
			// std::cin >> choice;
            double error = 0;
			iteration = 0;
            epoch++;
            if (debug) std::cout << "epoch: " << epoch << std::endl;
		}
	}
}

int Network::predict(std::vector<double> input){
    feedForward(input);
    std::vector<double> output = getOutput();
    int prediction = getMaxActivationIndex(output);
    return prediction;
}