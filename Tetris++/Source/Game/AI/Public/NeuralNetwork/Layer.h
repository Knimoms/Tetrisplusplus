#pragma once
#include <memory>
#include <vector>

class Matrix;
class Neuron;

class Layer
{
private:
    std::pmr::vector<std::shared_ptr<Neuron>> m_Neurons;

public:
    Layer(unsigned int numNeurons);

public:
    void SetNeuronValueAtIndex(unsigned int index, double inValue) const;
    std::shared_ptr<Matrix> GetValueMatrix() const;
    std::shared_ptr<Matrix> GetActivatedValueMatrix() const;
    std::shared_ptr<Matrix> GetDerivedValueMatrix() const;
};
