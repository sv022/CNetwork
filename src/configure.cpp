#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdint>

uint32_t bswap32(uint32_t x) {
    return ((x & 0xFF000000) >> 24) |
           ((x & 0x00FF0000) >> 8)  |
           ((x & 0x0000FF00) << 8)  |
           ((x & 0x000000FF) << 24);
}

std::vector<std::vector<double>> load_dataset(std::string path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }
    file.ignore(4);
    int num_images = 0;
    int num_rows = 0;
    int num_cols = 0;
    
    file.read((char*)&num_images, 4);
    num_images = bswap32(num_images);
    file.read((char*)&num_rows, 4);
    num_rows = bswap32(num_rows);
    file.read((char*)&num_cols, 4);
    num_cols = bswap32(num_cols);

    std::vector<std::vector<double>> images(num_images, std::vector<double>(num_rows * num_cols));

    for (int i = 0; i < num_images; i++) {
        std::vector<unsigned char> raw_image(num_rows * num_cols);
        file.read((char*)raw_image.data(), num_rows * num_cols);

        for (int j = 0; j < num_rows * num_cols; j++) {
            images[i][j] = raw_image[j] / 255.0;
        }
    }

    file.close();
    return images;
}

std::vector<unsigned char> load_labels(std::string path) {
    std::ifstream file(path, std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    file.ignore(4);

    int num_labels = 0;
    file.read((char*)&num_labels, 4);
    num_labels = bswap32(num_labels);

    std::vector<unsigned char> labels(num_labels);
    file.read((char*)labels.data(), num_labels);

    file.close();
    return labels;
}
