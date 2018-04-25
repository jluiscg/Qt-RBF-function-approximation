/**
 *Authors: José Luis Cortés Gutiérrez 
 *		   Carlos Andrés Hernández Blanco
 *Radial neuron class' implementation
 *Date: November 23, 2017
*/
#include "radialneuron.h"

RadialNeuron::RadialNeuron(double x)
{
    this->x_=x;
    this->spread_=0.3; //default value just for initialization view
}
double RadialNeuron::EuclideanDistance(double x){
    return qSqrt(qPow(this->x_ - x, 2));
}
double RadialNeuron::Gaussian(double x){
    return qExp( -(qPow(x - this->x_,2)) / ( 2 * qPow(this->spread_,2) ) );
}
