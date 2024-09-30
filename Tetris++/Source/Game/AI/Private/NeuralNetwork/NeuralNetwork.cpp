#include "NeuralNetwork/NeuralNetwork.h"
#include "NeuralNetwork/Layer.h"
#include "NeuralNetwork/Matrix.h"

#include "json/json.hpp"
#include <fstream>
#include <iostream>

void NeuralNetwork::SetCurrentInput(const std::vector<double>& input)
{
    const int inputSize = (int)input.size();

    if (m_Layers.empty() || m_Layers[0]->GetNeuronsNum() < inputSize)
        return;

    m_CurrentInput = input;

    for (int i = 0; i < inputSize; ++i)
        m_Layers[0]->SetNeuronValueAtIndex(i, input[i]);
}

std::vector<double> NeuralNetwork::GetOutputVector() const
{
    if (m_Layers.empty())
        return {};

    int neuronsNum = (int)m_Layers.size();

    auto outputMatrix = m_Layers[neuronsNum - 1]->GetActivatedValueMatrix();
    std::vector<double> outputVector;

    for (unsigned int i = 0; i < outputMatrix->GetNumColumns(); ++i)
        outputVector.push_back(outputMatrix->GetValue(0, i));

    return outputVector;
}

std::shared_ptr<Matrix> NeuralNetwork::GetOutputMatrix() const
{
    if (m_Layers.empty())
        return nullptr;

    return m_Layers[m_Layers.size() - 1]->GetActivatedValueMatrix();
}

int NeuralNetwork::GetHighestOutputValueIndex() const
{
    auto outputMatrix = GetOutputMatrix();

    if(!outputMatrix)
        return -1;

    int highestValueIndex = -1;
    double highestValue = -1.;

    unsigned int columnsNum = outputMatrix->GetNumColumns();
    
    for(unsigned int i = 0; i < columnsNum; ++i)
    {
        double currentValue = outputMatrix->GetValue(0, i);
        if(highestValueIndex < 0 || currentValue > highestValue)
        {
            highestValue = currentValue;
            highestValueIndex = (int)i;
        }
    }

    return highestValueIndex;
}

bool IsFileOlderThan(const std::filesystem::path& inFile, const std::filesystem::path& compareFile)
{
    return std::filesystem::last_write_time(inFile) <
        std::filesystem::last_write_time(compareFile);
}

std::filesystem::path GetLastFileInDirectory(const std::string& directoryPath,
                                             const std::string& fileNameSubstring = "")
{
    std::filesystem::path latestFile;

    for (auto& entry : std::filesystem::directory_iterator(directoryPath))
        if ((fileNameSubstring.empty() || entry.path().string().find(fileNameSubstring) != std::string::npos) &&
            (latestFile.empty() || IsFileOlderThan(latestFile, entry.path())))
            latestFile = entry.path();

    std::cout << latestFile << " " << fileNameSubstring << std::endl;
    return latestFile;
}


NeuralNetwork::NeuralNetwork()
{
    Load(GetLastFileInDirectory(s_SaveFolder, s_AutoSavePrefix).string());
}

NeuralNetwork::NeuralNetwork(const std::vector<int>& topology)
{
    GenerateLayersAndMatrices(topology);
}

NeuralNetwork::NeuralNetwork(const std::vector<int>& topology, const std::string& filePrefix)
    :m_FilePrefix(filePrefix)
{
    Load(GetLastFileInDirectory(s_SaveFolder, filePrefix).string());

    if(m_Layers.empty())
        GenerateLayersAndMatrices(topology);    
}

NeuralNetwork::NeuralNetwork(const std::string& fileName)
{
    Load(fileName);
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

void NeuralNetwork::GenerateLayersAndMatrices(const std::vector<int>& topology)
{
    const int maxTopIndex = (int)topology.size() - 1;

    for (int i = 0; i < maxTopIndex; ++i)
    {
        m_WeightMatrices.push_back(std::make_shared<Matrix>(topology[i], topology[i + 1], true));
        m_Layers.push_back(std::make_shared<Layer>(topology[i]));
    }

    m_Layers.push_back(std::make_shared<Layer>(topology[maxTopIndex]));
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

std::string NeuralNetwork::s_SaveFolder = "Save/AI/NeuralNetwork/";
std::string NeuralNetwork::s_AutoSavePrefix = "NN_autosave_";

void NeuralNetwork::AutoSave() const
{
    std::ostringstream outStringStream;

    outStringStream << std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    Save(s_AutoSavePrefix + outStringStream.str());
}

void NeuralNetwork::Save(const std::string& fileName) const
{
    std::ofstream outFilestream(s_SaveFolder + fileName + ".json");

    int weightMatricesNum = (int)m_WeightMatrices.size();
    int layersNum = (int)m_Layers.size();

    nlohmann::json saveJSON;
    saveJSON["layers"] = layersNum;

    for (int i = 0; i < layersNum; ++i)
        saveJSON["layer" + std::to_string(i)] = m_Layers[i]->GetNeuronsNum();

    for (int i = 0; i < weightMatricesNum; ++i)
    {
        auto currentMatrix = m_WeightMatrices[i];
        std::string matrixId = "matrix" + std::to_string(i);

        for (unsigned int row = 0; row < currentMatrix->GetNumRows(); ++row)
            for (unsigned int column = 0; column < currentMatrix->GetNumColumns(); ++column)
                saveJSON[matrixId][std::to_string(row) + std::to_string(column)] = currentMatrix->GetValue(
                    row, column);
    }

    outFilestream << saveJSON << std::endl;
    outFilestream.close();
}

void NeuralNetwork::Load(const std::string& fileName)
{
    std::ifstream inFilestream(s_SaveFolder + fileName + ".json");

    if (inFilestream.fail())
        return;

    m_Layers.clear();
    m_WeightMatrices.clear();

    nlohmann::json saveJSON;
    inFilestream >> saveJSON;

    const int maxLayersIndex = (int)saveJSON["layers"] - 1;

    for (int i = 0; i < maxLayersIndex; ++i)
    {
        const int neuronsInLayer = saveJSON["layer" + std::to_string(i)];
        auto currentMatrix = std::make_shared<
            Matrix>(neuronsInLayer, saveJSON["layer" + std::to_string(i + 1)], false);

        m_WeightMatrices.push_back(currentMatrix);
        m_Layers.push_back(std::make_shared<Layer>(neuronsInLayer));

        std::string matrixId = "matrix" + std::to_string(i);

        for (unsigned int row = 0; row < currentMatrix->GetNumRows(); ++row)
            for (unsigned int column = 0; column < currentMatrix->GetNumColumns(); ++column)
                currentMatrix->SetValue(row, column,
                                        saveJSON[matrixId][std::to_string(row) + std::to_string(column)]);
    }

    m_Layers.push_back(std::make_shared<Layer>(saveJSON["layer" + std::to_string(maxLayersIndex)]));
    inFilestream.close();
}
