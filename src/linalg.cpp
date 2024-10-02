#pragma once
#include<vector>
#include <cstdlib>

int randint(int a, int b){
    return a + (std::rand() % (b - a));
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
};