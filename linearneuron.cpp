/**
 *Authors:
 *  José Luis Cortés Gutiérrez 
 *  Carlos Andrés Hernández Blanco
 *Linear neuron class' implementation
 *Date: November 23, 2017
*/
#include "linearneuron.h"

LinearNeuron::LinearNeuron()
{

}
LinearNeuron::LinearNeuron(int dimension)
{
    for(int i=1;i<=dimension;i++){
        pesos_.append((qrand()%5)+(i/10));
    }
    bias_=1;
}
//Z
double LinearNeuron::Output(QVector<double> x){
    double z=0;
    //Wkj*Phi(rj) Summation
    for(int i=0;i<pesos_.size();i++){
        z+=pesos_.at(i)*x.at(i);
    }
    z+=bias_;
    return z;
}
