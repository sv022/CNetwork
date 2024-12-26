#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include"src/network.cpp"


int main() {
	srand(time(NULL));
    int layers_count = 3;
    std::vector<int> layers = {784, 256, 10};

	Network net(layers_count, layers);
    net.train("data3000.txt", 10);
    
    std::cout << net.test("data3000.txt", 50);
}