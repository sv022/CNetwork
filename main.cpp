#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include"src/network.cpp"


int main() {
	srand(time(NULL));
    int layers_count = 4;
    std::vector<int> layers = {1024, 512, 128, 10};

	Network net(layers_count, layers);
    net.train("data.txt", 25);

    File testFile("data.txt");

    int testIndex = 35;
    
    std::vector<double> input = testFile.getInputs(testIndex);
    std::vector<double> target = testFile.getTargets(testIndex);
    for (int i = 0; i < target.size(); i++) std::cout << target[i] << ' ';
    std::cout << "guess: " << net.predict(input) << '\n';
}