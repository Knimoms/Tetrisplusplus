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

    std::vector<double> m_CurrentInput;

public:
    void SetCurrentInput(const std::vector<double>& input);

public:
    NeuralNetwork(const std::vector<int>& topology);
    NeuralNetwork(const std::string& fileName);

public:
    void FeedForward() const;
    std::string ToString() const;

private:
    static std::string s_SaveFolder;

public:
    void Save(const std::string& fileName) const;
    void Load(const std::string& fileName);
};
