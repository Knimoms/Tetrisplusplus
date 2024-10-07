#include "NeuralNetwork/Layer.h"
#include "NeuralNetwork/Matrix.h"
#include "NeuralNetwork/Neuron.h"

Layer::Layer(unsigned int numNeurons)
{
    for (unsigned int i = 0; i < numNeurons; ++i)
    {
        m_Neurons.push_back(std::make_shared<Neuron>(0));
    }
}

void Layer::SetNeuronValueAtIndex(unsigned int index, double inValue) const
{
    if (index >= m_Neurons.size())
        return;

    m_Neurons[index]->SetValue(inValue);
}

std::shared_ptr<Matrix> Layer::GetValueMatrix() const
{
    int neuronsNum = (int)m_Neurons.size();
    auto matrix = std::make_shared<Matrix>(1, neuronsNum, false);

    for (int i = 0; i < neuronsNum; ++i)
        matrix->SetValue(0, i, m_Neurons[i]->GetValue());

    return matrix;
}

std::shared_ptr<Matrix> Layer::GetActivatedValueMatrix() const
{
    int neuronsNum = (int)m_Neurons.size();
    auto matrix = std::make_shared<Matrix>(1, neuronsNum, false);

    for (int i = 0; i < neuronsNum; ++i)
        matrix->SetValue(0, i, m_Neurons[i]->GetActivatedValue());

    return matrix;
}