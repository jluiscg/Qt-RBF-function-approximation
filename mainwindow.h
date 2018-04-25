/**
 *Authors: José Luis Cortés Gutiérrez 
 *		   Carlos Andrés Hernández Blanco
 *Definition for all functions needed in mainwindow
 *Date: November 23, 2017
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QVector>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QtMath>
#include <QPointF>
#include <QString>
#include <QDebug>
#include "radialneuron.h"
#include "linearneuron.h"

using namespace QtCharts;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_bttnEntrenar_clicked();

    void on_rbSeno_clicked();

    void on_rbCoseno_clicked();

    void on_bttnInicializar_clicked();

    void on_rbFuncion3_clicked();

    void on_rbFuncion4_clicked();

    void on_rbFuncion5_clicked();

private:
    Ui::MainWindow *ui;
    QLineSeries *series;
    //function chart
    QChart *chart;
    QChartView *chartView;
    QGridLayout* gridLayout;
    //gaussians chart
    QChart *chart_gaussians;
    QChartView *chartView_gaussians;
    QGridLayout* gridLayout_gaussians;
    //error chart
    QLineSeries* series_error;
    QChart *chart_error;
    QChartView *chartView_error;
    QGridLayout* gridLayout_error;
    int current_epoch;
    bool done;
    double learning_rate;
    int max_epochs;
    double error;
    double desired_error;
    double mse;
    int k;  //number of radial neurons
    QVector<double> desired_outputs;
    QVector<double> inputs;

    QVector<RadialNeuron*> radial_neurons;
    LinearNeuron linear_neuron;

    void Restart();
    void Delay();
    void Train();
    void UpdateGaussianChart();
    void UpdateErrorChart();
    void ProcessNewFunction(int option);
};

#endif // MAINWINDOW_H
