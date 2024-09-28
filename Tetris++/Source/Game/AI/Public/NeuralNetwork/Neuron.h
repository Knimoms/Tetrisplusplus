#pragma once

class Neuron
{
private:
    double m_Value;

public:
    inline double GetValue() const { return m_Value; }
    void SetValue(double inValue);

public:
    Neuron(double inValue);

public:
    double GetActivatedValue() const;
    double GetDerivedValue() const;
};
