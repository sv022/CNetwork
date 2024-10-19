#include"src/network.cpp"
#include <iostream>
#include "src/configure.cpp"
#include <algorithm>

// int main() {
//     std::string dataset_path = "D:/vuz/cn-dataset/train-images.idx3-ubyte";
//     std::string labels_path = "D:/vuz/cn-dataset/train-labels.idx1-ubyte";

//     int num_images = 10;
//     int image_size = 28 * 28; 

//     std::vector<std::vector<double>> normalized_dataset = load_dataset(dataset_path);
//     std::vector<unsigned char> labels = load_labels(labels_path);

//     Network network(2);

//     network.initialize_layer(5);
//     network.initialize_layer(2);

//     // network.forward(normalized_dataset[1]);

//     // std::vector<double> outp = network.get_output();
//     // for (int i = 0; i < (int)outp.size(); i++) {
//     //     std::cout << outp[i] << " ";
//     // }

//     std::string out_file_path = "weights1.bin";
//     network.save_weights(out_file_path);

//     Network network1(2);

//     network1.initialize_layer(5);
//     network1.initialize_layer(2);
//     network1.load_weights_biases("weights1.bin");
//     network1.save_weights("weights2.bin");
//     return 0;
// }

int main() {
    Network net(28 * 28);  // Example: 784 input neurons
    net.initialize_layer(256);  // Hidden layer with 10 neurons
    // net.initialize_layer(64);  // Hidden layer with 10 neurons
    net.initialize_layer(10);  // Output layer with 10 neurons

    // std::vector<double> input = {0.1, 0.2, 0.3};
    // std::vector<double> expected = {0.0, 1.0};
    int batch_size = 8;

    std::string dataset_path = "D:/vuz/cn-dataset/train-images.idx3-ubyte";
    std::string labels_path = "D:/vuz/cn-dataset/train-labels.idx1-ubyte";
    std::vector<std::vector<double>> normalized_dataset = load_dataset(dataset_path);
    std::vector<unsigned char> labels = load_labels(labels_path);

    std::vector<std::vector<double>> normalized_labels;

    net.forward(normalized_dataset[0]);
    std::vector<double> output = net.get_output();
    std::cout << "Predict with no education: ";
    for (double i : output) std::cout << i << " ";
    std::cout << std::endl; 

    net.save_weights("weights1.bin");

    double learning_rate = 0.01;
    int val;
    for (int epoch = 0; epoch < 10000; epoch++) {
            std::vector<double> current_label(10, 0.0);
            val = static_cast<int>(labels[epoch]);
            current_label[val] = 1.0;
            net.backprop(normalized_dataset[epoch], current_label, learning_rate);
            current_label[val] = 0.0;
        if ((epoch + 1) % 100 == 0) std::cout << "Epoch " << epoch + 1 << " has finished." << std::endl;
    }

    int correct = 0;
    std::vector<double> predict;
    int ans;
    for (int i = 0; i < 100; i++) {
        net.forward(normalized_dataset[i]);
        predict = net.get_output();
        ans = std::distance(predict.begin(), std::max_element(predict.begin(), predict.end()));
        std::cout << "Predict: " << ans << ", correct answer: " << static_cast<int>(labels[i]) << ", predict vector: ";
        for (double i : predict) std::cout << i << ' ';
        std::cout << std::endl;

        if (ans == static_cast<int>(labels[i])) {
            correct++;
        }
    }
    std::cout << "Precision: " << correct / 100.0 << std::endl;

    net.forward(normalized_dataset[0]);
    std::cout << "Predict after 10 epochs: ";
    output = net.get_output();
    for (double i : output) std::cout << i << " ";
    std::cout << std::endl;

    net.save_weights("weights2.bin");
}