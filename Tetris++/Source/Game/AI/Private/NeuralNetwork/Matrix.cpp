#include "NeuralNetwork/Matrix.h"

#include <format>
#include <random>

double Matrix::GetValue(unsigned int row, unsigned int column) const
{
    if (!(m_NumRows > row && m_NumColumns > column))
        return -1.87;

    return m_Values[column][row];
}

void Matrix::SetValue(unsigned int row, unsigned int column, double inValue)
{
    if (!(row < m_NumRows && column < m_NumColumns))
        return;

    m_Values[column][row] = inValue;
}

void Matrix::SetValues(const std::shared_ptr<Matrix>& valueMatrix, double mutationRate)
{
    if (valueMatrix->GetNumColumns() != m_NumColumns || valueMatrix->GetNumRows() != m_NumRows)
        return;

    for (unsigned int i = 0; i < m_NumColumns; ++i)
        for (unsigned int j = 0; j < m_NumRows; ++j)
        {
            double mutationRoll = GetRandomValue();

            if (mutationRoll > mutationRate)
            {
                SetValue(j, i, valueMatrix->GetValue(j, i));
                continue;
            }

            SetValue(j, i, GetRandomValue());
        }
}

Matrix::Matrix(unsigned int numRows, unsigned int numColumns, bool bRandom)
    : m_NumRows(numRows), m_NumColumns(numColumns)
{
    for (unsigned int i = 0; i < m_NumColumns; ++i)
    {
        std::vector<double> columnValues;
        for (unsigned int j = 0; j < m_NumRows; ++j)
        {
            columnValues.push_back(bRandom ? GetRandomValue() : 0.);
        }

        m_Values.push_back(columnValues);
    }
}

double Matrix::GetRandomValue()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0., 1.);

    return dis(gen);
}

std::shared_ptr<Matrix> Matrix::Transpose() const
{
    auto transposedMatrix = std::make_shared<Matrix>(m_NumColumns, m_NumRows, false);

    for (unsigned int i = 0; i < m_NumRows; ++i)
        for (unsigned int j = 0; j < m_NumColumns; ++j)
            transposedMatrix->SetValue(j, i, GetValue(i, j));

    return transposedMatrix;
}

std::string Matrix::ToString() const
{
    std::string outputString;

    for (unsigned int i = 0; i < m_NumRows; ++i)
    {
        for (unsigned int j = 0; j < m_NumColumns; ++j)
        {
            outputString += std::to_string(m_Values[j][i]);
            outputString += "\t";
        }
        outputString += "\n";
    }
    return outputString;
}

Matrix::operator std::vector<double>() const
{
    std::vector<double> result;

    for (unsigned int i = 0; i < m_NumRows; ++i)
        for (unsigned int j = 0; j < m_NumColumns; ++j)
            result.push_back(GetValue(i, j));

    return result;
}

std::shared_ptr<Matrix> Matrix::Multiply(std::shared_ptr<Matrix> matrix) const
{
    if (m_NumColumns != matrix->m_NumRows)
        return nullptr;

    auto outputMatrix = std::make_shared<Matrix>(m_NumRows, matrix->m_NumColumns, false);

    for (unsigned int i = 0; i < m_NumRows; ++i)
        for (unsigned int j = 0; j < matrix->m_NumColumns; ++j)
            for (unsigned int k = 0; k < m_NumColumns; ++k)
            {
                double result = GetValue(i, k) * matrix->GetValue(k, j);
                outputMatrix->SetValue(i, j, outputMatrix->GetValue(i, j) + result);
            }

    return outputMatrix;
}
