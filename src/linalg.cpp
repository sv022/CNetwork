#pragma once
#include<vector>
#include<cstdlib>
#include<math.h>

int randint(int a, int b){
    return a + (std::rand() % (b - a));
}

double sigmoid(double x){
    return 1 / (1 + exp(x));
}

double sigmoid_derivative(double x){
    double activation = sigmoid(x);
    return activation * (1 - activation);
}


std::vector<double> sigmoid(std::vector<double> input) {   
    int size = input.size();
    std::vector<double> output(size);
    
    for (int i = 0; i < size; i++)
        output[i] = 1 / (1 + exp(-input[i]));
    
    return output;
}

template<typename T>
class Matrix
{
    private:
    std::vector<std::vector<T>> self;

    public:
    std::pair<int, int> shape;

    Matrix(){
        shape.first = 0;
        shape.second = 0;
    }
    
    Matrix(int i, int j){
        shape.first = i;
        shape.second = j;
        for (int row = 0; row < i; row++) {
            self.push_back(std::vector<double>());
            for (int col = 0; col < j; col++){
                self[row].push_back(0);
            }
        }
    };

    std::vector<T> &operator[](int i){
        return self[i];
    }

    std::vector<T> &operator+(std::vector<T> other){
        if (shape.second != 1) throw std::invalid_argument("Invalid matrix dimensions");
        std::vector<T> result(shape.first);

        for (int i = 0; i < shape.first; i++)
            result[i][0] = self[i][0] + other[i];
            
        return result;
    }

    Matrix<T> &operator+(Matrix<T> other){
        if ((shape.first != other.shape.first) || (shape.second != other.shape.second)) 
            throw std::invalid_argument("Invalid matrix dimensions");
        Matrix<T> result(shape.first, shape.second);

        for (int i = 0; i < shape.first; i++){
            for (int j = 0; j < shape.second; j++){
                result[i][j] = self[i][0] + other[i];            
            }
        }

        return result;
    }

    void random(){
        for (int i = 0; i < shape.first; i++) {
            for (int j = 0; j < shape.second; j++) {
                self[i][j] = (double)randint(1, 100) / 100;
            }
        }
    }

    Matrix<T> dot(Matrix<T> other) {
        if (shape.second != other.shape.first) throw std::invalid_argument("Invalid matrix dimensions");
        Matrix<T> result(shape.first, other.shape.second);
        for (int i = 0; i < shape.first; i++){
            for (int j = 0; j < other.shape.second; j++){
                result[i][j] = 0;
                for (int k = 0; k < shape.second; k++) {
                    result[i][j] += self[i][k] * other.self[k][j];
                }
            }
        }
        return result;
    }

    std::vector<T> dot(std::vector<T> other) {
        if (shape.second != (int)other.size()) throw std::invalid_argument("Invalid matrix dimensions");
        std::vector<T> result(shape.first);
        for (int i = 0; i < shape.first; i++){
            result[i] = 0;
            for (int k = 0; k < shape.second; k++) {
                result[i] += self[i][k] * other[k];
            }
        }
        return result;
    }
};