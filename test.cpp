#include"src/network.cpp"
#include <iostream>
#include "src/configure.cpp"

int main() {
    std::string dataset_path = "D:/vuz/CNetwork/dataset/train-images.idx3-ubyte"; 
    std::string labels_path = "D:/vuz/CNetwork/dataset/train-labels.idx1-ubyte";

    int num_images = 10;
    int image_size = 28 * 28; 

    std::vector<std::vector<double>> normalized_dataset = load_dataset(dataset_path);
    std::vector<unsigned char> labels = load_labels(labels_path);

    Network network(2);

    network.initialize_layer(5);
    network.initialize_layer(2);

    // network.forward(normalized_dataset[1]);

    // std::vector<double> outp = network.get_output();
    // for (int i = 0; i < (int)outp.size(); i++) {
    //     std::cout << outp[i] << " ";
    // }

    std::string out_file_path = "weights1.bin";
    network.save_weights(out_file_path);

    Network network1(2);

    network1.initialize_layer(5);
    network1.initialize_layer(2);
    network1.load_weights_biases("weights1.bin");
    network1.save_weights("weights2.bin");
    return 0;
}