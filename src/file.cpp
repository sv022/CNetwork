#pragma once
#include<vector>
#include<string>
#include<fstream>
#include <sstream>


std::vector<std::string> split(std::string str, char c) {
	std::vector<std::string> array;
	std::string element = "";

	for (unsigned i = 0; i < str.length(); i++) {
		if (str[i] != c)
			element += str[i];
		else if (str[i] == c && element != "") {
			array.push_back(element);
			element = "";
		}
	} if (element != "")
		array.push_back(element);

	return array;
}

// Вспомогательный класс для чтения параметров сети и входных данных из файла
class File {
	
private:
	int MAX_ITERATIONS = 50;
	int DATA_SIZE;
	std::vector< std::vector<double> > inputs;
	std::vector< std::vector<double> > targets;

public:
	File(const char* filePath);
	std::vector<double> getInputs(const int index) const { return inputs[index]; };
	std::vector<double> getTargets(const int index) const { return targets[index]; };
	int getMaxIterations() const { return MAX_ITERATIONS; };
	int getDataSize() const { return DATA_SIZE; };

};


File::File(const char* filePath) {
	std::string line;
	std::vector<std::string> part;
	std::ifstream file(filePath);

	// чтение параметров из файла
	if (file.is_open()) {
		int index = 0;
		while (std::getline(file, line)) {
			if (index % 2 == 0) { // входные данные
				std::vector<double> input;
				part = split(line, ' ');
				for (unsigned p = 0; p < part.size(); p++)
					input.push_back(atof(part[p].c_str()));
				inputs.push_back(input);
			}
			else { // разметка входных данных
				std::vector<double> target;
				part = split(line, ' ');
				for (unsigned p = 0; p < part.size(); p++)
					target.push_back(atof(part[p].c_str()));
				targets.push_back(target);
			}
			index++;
		}
	}
	DATA_SIZE = inputs.size();
	file.close();
}