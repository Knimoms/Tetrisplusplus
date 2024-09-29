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
    std::vector<double> GetOutputVector() const;
    std::shared_ptr<Matrix> GetOutputMatrix() const;

public:
    NeuralNetwork();
    NeuralNetwork(const std::vector<int>& topology);
    NeuralNetwork(const std::string& fileName);

public:
    void FeedForward() const;
    std::string ToString() const;

private:
    static std::string s_SaveFolder;
    static std::string s_AutoSavePrefix;

public:
    void AutoSave() const;
    void Save(const std::string& fileName) const;
    void Load(const std::string& fileName);
};
