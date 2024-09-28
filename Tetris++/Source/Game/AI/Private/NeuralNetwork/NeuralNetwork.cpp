#include "NeuralNetwork/NeuralNetwork.h"
#include "NeuralNetwork/Layer.h"
#include "NeuralNetwork/Matrix.h"

#include "json/json.hpp"
#include <fstream>
#include <iostream>

void NeuralNetwork::SetCurrentInput(const std::vector<double>& input)
{
    m_CurrentInput = input;

    const int inputSize = (int)input.size();

    for (int i = 0; i < inputSize; ++i)
        m_Layers[0]->SetNeuronValueAtIndex(i, input[i]);
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

void NeuralNetwork::Save(const std::string& fileNameAppend) const
{
    std::ofstream o("Save/AI/NeuralNetwork/" + fileNameAppend + ".json");

    int weightMatricesNum = (int)m_WeightMatrices.size();
    int layersNum = (int)m_Layers.size();

    nlohmann::json saveJSON;
    saveJSON["layers"] = layersNum;

    for (int i = 0; i < layersNum; ++i)
        saveJSON["layer;" + std::to_string(i)] = m_Layers[i]->GetNeuronsNum();

    for (int i = 0; i < weightMatricesNum; ++i)
    {
        std::string matrixPrefix = "matrix;" + std::to_string(i) + ";";
        auto currentMatrix = m_WeightMatrices[i];

        saveJSON[matrixPrefix + "rows"] = currentMatrix->GetNumRows();
        saveJSON[matrixPrefix + "column"] = currentMatrix->GetNumColumns();

        for (unsigned int row = 0; row < currentMatrix->GetNumRows(); ++row)
            for (unsigned int column = 0; column < currentMatrix->GetNumColumns(); ++column)
                saveJSON[matrixPrefix + std::to_string(row) + ";" + std::to_string(column)] = currentMatrix->GetValue(
                    row, column);
    }

    o << saveJSON << std::endl;
    o.close();
}
