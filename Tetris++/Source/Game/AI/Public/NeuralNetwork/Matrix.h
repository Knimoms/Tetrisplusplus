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

public:
    Matrix(unsigned int numRows, unsigned int numColumns, bool bRandom);

protected:
    static double GetRandomValue();

public:
    std::shared_ptr<Matrix> Transpose() const;
    std::string ToString() const;

    std::shared_ptr<Matrix> operator*(const Matrix& matrix);
    explicit operator std::vector<double>() const;
};
