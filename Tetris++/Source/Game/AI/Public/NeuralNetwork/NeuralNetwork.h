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
    NeuralNetwork(const std::string& filePath);

public:
    void FeedForward() const;
    void GenerateLayersAndMatrices(const std::vector<int>& topology);
    std::string ToString() const;

private:
    static std::string s_SaveFolder;

protected:
    unsigned int GetHighestExistingGenerationIndex() const;

public:
    void AutoSave();
    void Save(const std::string& fileName);
    void Load(const std::string& filePath);

public:
    static std::string GetBestFile(const std::string& filePrefix);
    static void FilterGenerationsForBest(const std::string& filePrefix);

    static void ResetToBestGeneration(const std::string& filePrefix);
};
