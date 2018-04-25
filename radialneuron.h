/**
 *Authors:
 *  José Luis Cortés Gutiérrez 
 *  Carlos Andrés Hernández Blanco
 *Radial neuron class' Definition
 *Date: November 20, 2017
*/
#ifndef RADIALNEURON_H
#define RADIALNEURON_H

#include <QtCore/qmath.h>

class RadialNeuron
{
public:
    RadialNeuron(double x);
    double x_;
    double spread_;
    double EuclideanDistance(double x);
    double Gaussian(double x);
};

#endif // RADIALNEURON_H
