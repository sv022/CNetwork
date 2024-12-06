#pragma once
#include<vector>
#include <cmath>


class Neuron;
typedef std::vector<Neuron> Layer;

struct Connection {
	double weight;
	double deltaWeight;
};

class Neuron {

private:
	const double learningRate = 0.05;
	const double alpha = 0.1;
	//static double activate(double value) { return tanh(value); }
	static double activate(double value) { return 1 / (1 + exp(-value)); }
	//static double activateDerivative(double value) { return 1 - tanh(value) * tanh(value); }
	static double activateDerivative(double value) { return activate(value) * (1 - activate(value)); }
	static double random(void) { return rand() / double(RAND_MAX); }
	double sumDOW(const Layer &nextLayer) const;
	double output;
	std::vector<Connection> outputWeights;
	unsigned index;
	double gradient;

public:
	Neuron(unsigned outputSize, unsigned index);
	void setOutput(double value) { output = value; }
	double getOutput(void) const { return output; }
	std::vector<Connection> getOutputWeights() const { return outputWeights; }
	void feedForward(const Layer &prevLayer);
	void calculateOutputGradients(double target);
	void calculateHiddenGradients(const Layer &nextLayer);
	void updateWeights(Layer &prevLayer);

};


Neuron::Neuron(unsigned outputSize, unsigned index) {
	this->index = index;
	outputWeights.reserve(outputSize);

	for (unsigned i = 0; i < outputSize; i++) {
		outputWeights.push_back(Connection());
		outputWeights.back().weight = random();
	}
}

void Neuron::updateWeights(Layer &prevLayer) {
	for (unsigned n = 0; n < prevLayer.size(); n++) {

		// double oldDeltaWeight = prevLayer[n].outputWeights[index].deltaWeight;
		// double newDeltaWeight = learningRate * prevLayer[n].getOutput() * gradient + alpha * oldDeltaWeight;

		double newDeltaWeight = learningRate * prevLayer[n].getOutput() * gradient;

		prevLayer[n].outputWeights[index].deltaWeight = newDeltaWeight;
		prevLayer[n].outputWeights[index].weight += newDeltaWeight;
	}
}

// TODO: Add explanation
double Neuron::sumDOW(const Layer &nextLayer) const {
	double sum = 0.0;

	for (unsigned n = 0; n < nextLayer.size() - 1; n++)
		sum += outputWeights[n].weight * nextLayer[n].gradient;

	return sum;
}

void Neuron::calculateHiddenGradients(const Layer &nextLayer) {
	double dow = sumDOW(nextLayer);
	gradient = dow * Neuron::activateDerivative(output);
}

void Neuron::calculateOutputGradients(double target) {
	double delta = target - output;
	gradient = delta * Neuron::activateDerivative(output);
}

void Neuron::feedForward(const Layer &prevLayer) {
	double sum = 0.0;

	for (unsigned n = 0; n < prevLayer.size(); n++)
		sum += prevLayer[n].getOutput() * prevLayer[n].outputWeights[index].weight;

	output = Neuron::activate(sum);
}

