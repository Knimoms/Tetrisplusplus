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
