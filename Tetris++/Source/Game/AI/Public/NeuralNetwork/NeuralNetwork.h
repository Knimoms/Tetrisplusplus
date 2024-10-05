#pragma once
#include <memory>
#include <string>
#include <vector>

class Matrix;
class Layer;

class NeuralNetwork
{
private:
    std::vector<std::shared_ptr<Layer>> m_Layers;
    std::vector<std::shared_ptr<Matrix>> m_WeightMatrices;

    std::string m_FilePrefix = "NN_autosave_";

    std::vector<double> m_CurrentInput;

    unsigned int m_Generation;
    unsigned int m_GenerationIndex;
    
    double m_Fitness;

public:
    void SetCurrentInput(const std::vector<double>& input);
    std::vector<double> GetOutputVector() const;
    std::shared_ptr<Matrix> GetOutputMatrix() const;

    int GetHighestOutputValueIndex() const;

    void SetFitness(double inFitness);

public:
    NeuralNetwork();
    NeuralNetwork(const NeuralNetwork& parent, float mutationRate = 0.0f);
    NeuralNetwork(const std::vector<int>& topology);
    NeuralNetwork(const std::vector<int>& topology, const std::string& filePrefix);
    NeuralNetwork(const std::string& fileName);

public:
    void FeedForward() const;
    void GenerateLayersAndMatrices(const std::vector<int>& topology);
    std::string ToString() const;

private:
    static std::string s_SaveFolder;

protected:
    unsigned int GetHighestExistingGenerationIndex() const;

public:
    void AutoSave() const;
    void Save(const std::string& fileName) const;
    void Load(const std::string& fileName);
};
