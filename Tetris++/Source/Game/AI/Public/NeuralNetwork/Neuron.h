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

private:
    mutable double m_ValueOnLastOperation = -1.;
    mutable double m_CachedActivatedValue;

public:
    double GetActivatedValue() const;

private:
    mutable double m_ActivatedValueOnLastOperation = -1.;
    mutable double m_CachedDerivedValue;

public:
    double GetDerivedValue() const;
};
