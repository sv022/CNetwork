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
    net.train("data.txt", 35);
    
    std::cout << net.test("data.txt", 10);
}