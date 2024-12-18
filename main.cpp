#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include"src/network.cpp"


int main() {
	srand(time(NULL));
    int layers_count = 4;
    std::vector<int> layers = {784, 256, 32, 10};

	Network net(layers_count, layers);
    net.train("dataFashion.txt", 8);
    
    std::cout << net.test("dataFashion.txt", 50);
}