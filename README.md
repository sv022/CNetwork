# Нейронная сеть

Полносвязная нейронная сеть (многослойный перцептрон), реализация на языке C++.

## Содержание
- [Функционал](#функционал)
- [Модель](#модель)

## Функционал

Класс нейронной сети:

```cpp
Network(
    int layer_count, //число слоев (включая входной и скрытые слои)
    std::vector<int> layers_size // вектор размерности слоев от входного к выходному
);
```
```cpp
void train(
    std::string filePath, // путь к файлу с размеченными данными
    int epochs // число эпох обучения
);
```
```cpp
int predict(
    std::vector<double> input // входные значения
); // --> индекс нейрона с наибольшим значением (предсказание)
```

## Модель

Математическое описание устройства нейронной сети 

![Definitions](https://raw.githubusercontent.com/sv022/MockDB/refs/heads/main/CNetwork/def.png)

### Прямой проход
<br>

![forward](https://raw.githubusercontent.com/sv022/MockDB/refs/heads/main/CNetwork/forward.png)

[Cоответствующий код](src/network.cpp)
```
void Neuron::feedForward(const Layer &prevLayer)
```
<br>

### Обратный проход

Ошибка на выходном слое
<br>
![forward](https://raw.githubusercontent.com/sv022/MockDB/refs/heads/main/CNetwork/ErrorFunc.png)
<br>
Дельта на выходном слое
![forward](https://raw.githubusercontent.com/sv022/MockDB/refs/heads/main/CNetwork/OutputDelta.png)
<br>
Градиент на выходном слое
![forward](https://raw.githubusercontent.com/sv022/MockDB/refs/heads/main/CNetwork/OutputGrad.png)

<br>

[Cоответствующий код](src/neuron.cpp)

```cpp
void Neuron::updateWeights(Layer &prevLayer) {
    ...
    double newDeltaWeight = learningRate * prevLayer[n].getOutput() * gradient;
    ...
}
```
```cpp
void Neuron::calculateOutputGradients(double target) {
	double delta = target - output;
	gradient = delta * Neuron::activateDerivative(output);
}
```

Частные производные для весов скрытых слоев
<br>

![forward](https://raw.githubusercontent.com/sv022/MockDB/refs/heads/main/CNetwork/WeightDer.png)

<br>

[Cоответствующий код](src/neuron.cpp)

```cpp
double Neuron::sumDOW(const Layer &nextLayer) const {
	double sum = 0.0;
	for (unsigned n = 0; n < nextLayer.size(); n++)
		sum += outputWeights[n].weight * nextLayer[n].gradient;
	return sum;
}
```

```cpp
void Neuron::calculateHiddenGradients(const Layer &nextLayer) {
	double dow = sumDOW(nextLayer);
	gradient = dow * Neuron::activateDerivative(output);
}
```

Обновление весов

![forward](https://raw.githubusercontent.com/sv022/MockDB/refs/heads/main/CNetwork/WeightsUpdate.png)


[Cоответствующий код](src/neuron.cpp)

```cpp
void Neuron::updateWeights(Layer &prevLayer) {
    ...
    double newDeltaWeight = learningRate * prevLayer[n].getOutput() * gradient;

    prevLayer[n].outputWeights[index].deltaWeight = newDeltaWeight;
    prevLayer[n].outputWeights[index].weight += newDeltaWeight;
    ...
}
```

