#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include"src/network.cpp"


int main() {
	srand(time(NULL));
	File file("data.txt");

    int layers_count = 4;
    std::vector<int> layers = {1024, 512, 128, 10};

	Network net(layers_count, layers);
    net.train("data.txt", 25);

    File testFile("data.txt");
    
    std::vector<double> input = testFile.getInputs(6);
    std::vector<double> target = testFile.getTargets(6);
    for (int i = 0; i < target.size(); i++) std::cout << target[i] << ' ';
    std::cout << "guess: " << net.predict(input) << '\n';
}