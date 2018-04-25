/**
 *Authors:
 *  José Luis Cortés Gutiérrez 
 *  Carlos Andrés Hernández Blanco
 *Radial neuron class' definition
 *Date: November 20, 2017
*/
#ifndef LINEARNEURON_H
#define LINEARNEURON_H
#include <QVector>

class LinearNeuron
{
public:
    LinearNeuron();
    LinearNeuron(int dimension);
    QVector<double> pesos_;
    double bias_;
    double Output(QVector<double> x);
};

#endif // LINEARNEURON_H
