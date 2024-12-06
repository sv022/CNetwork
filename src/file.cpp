#pragma once
#include<vector>
#include<string>
#include<fstream>
#include <sstream>
#include <iostream>


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
	int MAX_ITERATIONS = 20;
	int DATA_SIZE;
	std::vector< std::vector<double> > inputs;
	std::vector< std::vector<double> > targets;

public:
	File(std::string filePath);
	std::vector<double> getInputs(const int index) const { return inputs[index]; };
	std::vector<double> getTargets(const int index) const { return targets[index]; };
	int getMaxIterations() const { return MAX_ITERATIONS; };
	int getDataSize() const { return DATA_SIZE; };

	void readFile(std::string filePath);

};


// File::File(std::string dirPath) {
// 	std::ifstream files(dirPath + "/files.txt");

// 	std::string fileName;

// 	std::string line;
// 	std::vector<std::string> part;

// 	std::vector<std::string> inputFiles;

// 	while (std::getline(files, fileName)) {
// 		std::vector<double> input;
// 		std::vector<double> target;

// 		std::ifstream file(dirPath + "/" + fileName + ".txt");
// 		if (!file.is_open()) continue;

// 		std::getline(file, line);

// 		part = split(line, ' ');
// 		for (unsigned p = 0; p < part.size(); p++)
// 			input.push_back(atof(part[p].c_str()));
// 		inputs.push_back(input);

// 		std::getline(file, line);
// 		part = split(line, ' ');
// 		for (unsigned p = 0; p < part.size(); p++)
// 			target.push_back(atof(part[p].c_str()));
// 		targets.push_back(target);

// 		file.close();
// 	}

// 	files.close();
// 	DATA_SIZE = inputs.size();
// }

File::File(std::string filePath) {
	std::string line;
	std::vector<std::string> part;
	std::ifstream file(filePath);

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