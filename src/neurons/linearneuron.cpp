/**
 *Authors:
 *  José Luis Cortés Gutiérrez 
 *Linear neuron class' implementation
 *Date: November 23, 2017
*/
#include "linearneuron.h"
#include <QRandomGenerator>

LinearNeuron::LinearNeuron()
{

}
LinearNeuron::LinearNeuron(int dimension)
{
    for(int i=1;i<=dimension;i++){
        weights_.append(QRandomGenerator::global()->bounded(5) + (i / 10.0));
    }
    bias_=1;
}
//Z
double LinearNeuron::Output(QVector<double> x){
    double z=0;
    //Wkj*Phi(rj) Summation
    for(int i=0;i<weights_.size();i++){
        z+=weights_.at(i)*x.at(i);
    }
    z+=bias_;
    return z;
}
