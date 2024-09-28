#include "NeuralNetwork/Neuron.h"

#include <iostream>
#include <cmath>

void Neuron::SetValue(double inValue)
{
    m_Value = inValue;
}

Neuron::Neuron(double inValue)
   : m_Value(inValue)
{
}

double Neuron::GetActivatedValue() const
{
    if (m_Value != m_ValueOnLastOperation)
    {
        m_ValueOnLastOperation = m_Value;
        m_CachedActivatedValue = m_Value / (1 + abs(m_Value));
    }

    return m_CachedActivatedValue;
}

double Neuron::GetDerivedValue() const
{
    double activatedValue = GetActivatedValue();

    if (activatedValue != m_ActivatedValueOnLastOperation)
    {
        m_ActivatedValueOnLastOperation = activatedValue;
        m_CachedDerivedValue = activatedValue * (1 - activatedValue);
    }

    return m_CachedDerivedValue;
}
