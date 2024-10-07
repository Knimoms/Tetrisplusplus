#pragma once
#include <ostream>
#include <vector>

class Matrix
{
private:
    unsigned int m_NumRows;
    unsigned int m_NumColumns;

    std::vector<std::vector<double>> m_Values;

public:
    inline unsigned int GetNumRows() const { return m_NumRows; }
    inline unsigned int GetNumColumns() const { return m_NumColumns; }
    
    double GetValue(unsigned int row, unsigned int column) const;
    void SetValue(unsigned int row, unsigned int column, double inValue);

    void SetValues(const std::shared_ptr<Matrix>& valueMatrix, double mutationRate = 0.f);

public:
    Matrix(unsigned int numRows, unsigned int numColumns, bool bRandom);

protected:
    static double GetRandomValue(double bottom = 0., double top = 1.);

public:
    std::string ToString() const;

    explicit operator std::vector<double>() const;

    std::shared_ptr<Matrix> Multiply(std::shared_ptr<Matrix> matrix) const;
};
