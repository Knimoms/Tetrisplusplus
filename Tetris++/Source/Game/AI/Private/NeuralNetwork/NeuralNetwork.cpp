#include "NeuralNetwork/NeuralNetwork.h"
#include "NeuralNetwork/Layer.h"
#include "NeuralNetwork/Matrix.h"

#include <iostream>

void NeuralNetwork::SetCurrentInput(const std::vector<double>& input)
{
    m_CurrentInput = input;

    const int inputSize = (int)input.size();

    for (int i = 0; i < inputSize; ++i)
    {
        m_Layers[0]->SetNeuronValueAtIndex(i, input[i]);
    }
}

NeuralNetwork::NeuralNetwork(const std::vector<int>& topology)
{
    const int maxTopIndex = (int)topology.size() - 1;

    for (int i = 0; i < maxTopIndex; ++i)
    {
        m_WeightMatrices.push_back(std::make_shared<Matrix>(topology[i], topology[i + 1], true));
        m_Layers.push_back(std::make_shared<Layer>(topology[i]));
    }

    m_Layers.push_back(std::make_shared<Layer>(topology[maxTopIndex]));
}

void NeuralNetwork::FeedForward() const
{
    int layersNum = (int)m_Layers.size();
    
    std::cout << m_Layers[layersNum-1]->GetActivatedValueMatrix()->ToString() << "\n";
    
    for (int i = 0; i < layersNum - 1; ++i)
    {
        auto neuronMatrix = i ? m_Layers[i]->GetActivatedValueMatrix() : m_Layers[i]->GetValueMatrix();
        auto weightMatrix = m_WeightMatrices[i];

        auto resultMatrix = *neuronMatrix * *weightMatrix;

        unsigned int numColums = resultMatrix->GetNumColumns();
        for (unsigned int j = 0; j < numColums; ++j)
        {
            m_Layers[i + 1]->SetNeuronValueAtIndex(j, resultMatrix->GetValue(0, j));
        }
    }
    
    std::cout << "----------------------------------\n";
    std::cout << m_Layers[layersNum-1]->GetActivatedValueMatrix()->ToString() << "\n";
    
}

std::string NeuralNetwork::ToString() const
{
    std::string outputString;

    const int numLayers = (int)m_Layers.size();

    if (!numLayers)
        return outputString;

    outputString += m_Layers[0]->GetValueMatrix()->ToString();

    for (int i = 1; i < numLayers; ++i)
        outputString += m_Layers[i]->GetActivatedValueMatrix()->ToString();

    return outputString;
}
