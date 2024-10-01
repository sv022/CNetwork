class Neuron
{
    private:
    double activation;

    public:
    Neuron(){
        activation = 0;
    }
    Neuron(double value){
        activation = value;
    }

    double get(){
        return activation;
    }

    void set(double value){
        activation = value;
    }
};