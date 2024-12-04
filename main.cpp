#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include"src/network.cpp"


int main() {
	srand(time(NULL));
    int layers_count = 4;
    std::vector<int> layers = {307200, 1024, 128, 2};

	Network net(layers_count, layers);
    net.train("image", 20);
}