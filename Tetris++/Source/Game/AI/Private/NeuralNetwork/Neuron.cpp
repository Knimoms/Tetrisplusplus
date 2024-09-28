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
    static double valueOnLastOperation = -1.;
    static double cachedActivatedValue = -1.;

    if (m_Value != valueOnLastOperation)
    {
        valueOnLastOperation = m_Value;
        cachedActivatedValue = m_Value / (1 + abs(m_Value));
    }

    return cachedActivatedValue;
}

double Neuron::GetDerivedValue() const
{
    static double activatedValueOnLastOperation = -1.;
    static double cachedDerivedValue = -1.;

    double activatedValue = GetActivatedValue();

    if (activatedValue != activatedValueOnLastOperation)
    {
        activatedValueOnLastOperation = activatedValue;
        cachedDerivedValue = activatedValue * (1 - activatedValue);
    }

    return cachedDerivedValue;
}
