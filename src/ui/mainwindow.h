/**
 *Authors:
 *  José Luis Cortés Gutiérrez 
 *Definition for all functions needed in mainwindow
 *Date: November 23, 2017
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChart>

QT_USE_NAMESPACE
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
    void on_btnTrain_clicked();

    void on_rbSine_clicked();

    void on_rbCosine_clicked();

    void on_btnInitialize_clicked();

    void on_rbFunction3_clicked();

    void on_rbFunction4_clicked();

    void on_rbFunction5_clicked();

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
