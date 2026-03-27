/**
 *Authors:
 *  José Luis Cortés Gutiérrez 
 *Radial neuron class' Definition
 *Date: November 20, 2017
*/
#ifndef RADIALNEURON_H
#define RADIALNEURON_H

#include <QtMath>

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
