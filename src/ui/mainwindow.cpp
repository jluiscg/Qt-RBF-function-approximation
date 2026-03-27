/**
 *Authors:
 *  José Luis Cortés Gutiérrez 
 *Implementation  for all functions needed in mainwindow
 *Date: November 23, 2017
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#define STEP_POINT 0.07
#define INPUT_RANGE 10
#define OP_SIN 1
#define OP_COS 2
#define OP_FUNCTION_3 3
#define OP_FUNCTION_4 4
#define OP_FUNCTION_5 5

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //functions
    series = new QLineSeries();
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Chart");
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    gridLayout = new QGridLayout(ui->widget);
    gridLayout->addWidget(chartView,0,0);
    //gaussians
    chart_gaussians = new QChart();
    chart_gaussians->legend()->hide();
    chart_gaussians->addSeries(new QLineSeries());
    chart_gaussians->createDefaultAxes();
    chart_gaussians->setTitle("Gaussians");
    chartView_gaussians = new QChartView(chart_gaussians);
    chartView_gaussians->setRenderHint(QPainter::Antialiasing);
    gridLayout_gaussians = new QGridLayout(ui->widgetGaussians);
    gridLayout_gaussians->addWidget(chartView_gaussians,0,0);
    //error
    series_error = new QLineSeries();
    chart_error = new QChart();
    chart_error->legend()->hide();
    chart_error->addSeries(series_error);
    chart_error->createDefaultAxes();
    chart_error->setTitle("Error");
    chartView_error = new QChartView(chart_error);
    chartView_error->setRenderHint(QPainter::Antialiasing);
    gridLayout_error = new QGridLayout(ui->widget_error);
    gridLayout_error->addWidget(chartView_error,0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete series;
    delete series_error;
}

void MainWindow::Restart()
{
    current_epoch=1;
    done=false;
    series->clear();
    desired_outputs.clear();
    inputs.clear();
    radial_neurons.clear();
    series_error->clear();
}

void MainWindow::on_btnTrain_clicked()
{
    //learning rate
    learning_rate=ui->lineLearning->text().toDouble();
    if(learning_rate==0){
        learning_rate=1;
        ui->lineLearning->setText("1.0");
    }
    //maximum epochs
    max_epochs=ui->lineEpochs->text().toInt();
    if(max_epochs==0){
        max_epochs=100;
        ui->lineEpochs->setText("100");
    }
    //desired error
    desired_error=ui->lineDesiredError->text().toDouble();
    if(desired_error==0){
        desired_error=0.0001;
        ui->lineDesiredError->setText("0.0001");
    }
    //number of Gaussian neurons
    k=ui->lineK->text().toInt();
    if(k==0){
        k=5;
        ui->lineK->setText("5");
    }
    //initialize whether a solution was found
    done = false;
    //initialize mean squared error
    mse=1;
    //initialize current epoch
    current_epoch=1;
    Train();
}

void MainWindow::Train()
{
    //stores the neurons' index to which belongs the inputs
    QVector<int> domains;
    //minimum value to determine domains
    double min_value;
    //neuron's index of resulting domain
    int min_value_index;
    //used to calculate the new centroid
    double average_x;
    //number of inputs that belong to a neuron's domain
    int num_of_hits;
    double euclidean_distance;
    //list of new centroids' value
    QVector<double> new_centroids;

    //First phase
    //calculate euclidean distances for all centroids
    for(int j=0; j<inputs.size();j++){
        for(int i=0; i<radial_neurons.size();i++){
            if(i==0){
                min_value =
                    radial_neurons.at(i)->EuclideanDistance(inputs.at(j));
                min_value_index = i;
            }
            else{
                euclidean_distance =
                    radial_neurons.at(i)->EuclideanDistance(inputs.at(j));
                if(euclidean_distance < min_value){
                    min_value = euclidean_distance;
                    min_value_index = i;
                }
            }
        }
        domains.append(min_value_index);
    }
    done = false;
    while(!done){
        new_centroids.clear();
        //calculate the new values for centroids with the average
        for(int i=0;i<radial_neurons.size();i++){
            average_x = 0;
            num_of_hits = 0;
            for(int j=0;j<domains.size();j++){
                if(domains.at(j) == i) {
                    average_x += inputs.at(j);
                    num_of_hits++;
                }
            }
            average_x = average_x / num_of_hits;
            new_centroids.append(average_x);
        }

        //Check if the new centroids are all equal to the old ones
        //Make "done" temporarily true so if any value is different
        //is simply changed to false and the loop is stopped
        done = true;
        for(int i=0;i<radial_neurons.size();i++){
            double newd=new_centroids.at(i);
            double old=radial_neurons.at(i)->x_;
            //compare if two double values are equal
            //tolerating a 0.001 precision error
            if(qAbs(newd - old) > (0.0001)){
                done = false;
                //break;
            }
        }
        //apply new values if they're different
        if(!done){
            for(int i=0;i<radial_neurons.size();i++){
                radial_neurons.at(i)->x_ = new_centroids.at(i);
            }
        }
    }
    //Second phase
    //set spread (sigma) of each neuron
    for(int i=0;i<radial_neurons.size();i++){
        RadialNeuron* temp = radial_neurons.at(i);
        radial_neurons.remove(i);
        for(int j=0;j<radial_neurons.size();j++){
            euclidean_distance =
                    temp->EuclideanDistance(radial_neurons.at(j)->x_);
            if(j==0){
                min_value = euclidean_distance;
            }
            else if(euclidean_distance < min_value){
                    min_value = euclidean_distance;
            }
        }
        radial_neurons.insert(i,temp);
        temp->spread_ = min_value;
    }
    //Third Phase
    QVector<double> radial_layer_output;
    QVector<double> Z;
    double output;
    double new_weight;
    while((current_epoch <= max_epochs) && (mse > desired_error)){
        //radial layer output
        Z.clear();
        mse=0;
        for(int i=0;i<inputs.size();i++){  //for each input
            radial_layer_output.clear();
            for(int j=0;j<radial_neurons.size();j++){  //for each radial neuron
                //calculate and save its output
                output = radial_neurons.at(j)->Gaussian(inputs.at(i));
                radial_layer_output.append(output);
            }

            //calculate the final output given by the linear combination of
            //weights * radial_output plus the bias
            output = 0;
            for(int j=0;j<linear_neuron.weights_.size();j++){
                output += linear_neuron.weights_.at(j) * radial_layer_output.at(j);
            }
            output += linear_neuron.bias_;
            Z.append(output);

            error = desired_outputs.at(i) - Z.at(i);  //pattern error;
            mse+=error;
            //update weights
            for(int j=0;j<linear_neuron.weights_.size();j++){
                new_weight = learning_rate * error * radial_layer_output.at(j);
                new_weight = linear_neuron.weights_.at(j) + new_weight;
                linear_neuron.weights_.replace(j, new_weight);
            }
        }
        mse=mse*mse/2;
        series_error->append(current_epoch,mse);
        //error chart
        chart_error->removeSeries(series_error);
        chart_error->removeAllSeries();
        chart_error->addSeries(series_error);
        chart_error->createDefaultAxes();
        //-----------
        ui->lblCurrentEpoch->setText(QString::number(current_epoch));
        chart_gaussians->removeAllSeries();
        QVector<QLineSeries *> seriesAux;
        for(int i=0;i<radial_layer_output.size();i++)
        {
            QLineSeries *serieAux = new QLineSeries();
            for(int j=0;j<domains.size();j++){
                if(domains.at(j) == i){
                    serieAux->append(inputs.at(j), Z.at(j));
                }
            }
            seriesAux.append(serieAux);
        }
        int numSeries=seriesAux.size();
        for(int i=0;i<numSeries;i++)
            chart_gaussians->addSeries(seriesAux.at(i));
        chart_gaussians->createDefaultAxes();
        UpdateGaussianChart();
        UpdateErrorChart();
        qApp->processEvents();
        Delay();
        current_epoch++;
    }
}

void MainWindow::Delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}
void MainWindow::UpdateErrorChart()
{
    gridLayout_error->removeWidget(chartView_error); // remove chart view from the layout
    chart_error->removeSeries(series_error); // keep series alive when deleting chart
    delete chart_error;
    delete chartView_error;
    chart_error = new QChart();
    chart_error->legend()->hide();
    chart_error->addSeries(series_error);
    chart_error->createDefaultAxes();
    chart_error->setTitle("Error");
    chartView_error = new QChartView(chart_error);
    chartView_error->setRenderHint(QPainter::Antialiasing);
    gridLayout_error->addWidget(chartView_error,0,0);
}
void MainWindow::UpdateGaussianChart()
{
    gridLayout_gaussians->removeWidget(chartView_gaussians); // remove chart view from the layout
    QList<QAbstractSeries*> tmp = chart_gaussians->series();
    for(int i=0;i<tmp.size();i++){
        chart_gaussians->removeSeries(tmp.at(i));
    }
    delete chart_gaussians;
    delete chartView_gaussians;
    chart_gaussians = new QChart();
    chart_gaussians->legend()->hide();
    for(int i=0;i<tmp.size();i++){
        chart_gaussians->addSeries(tmp.at(i));
    }
    chart_gaussians->createDefaultAxes();
    chart_gaussians->setTitle("Gaussians");
    chartView_gaussians = new QChartView(chart_gaussians);
    chartView_gaussians->setRenderHint(QPainter::Antialiasing);
    gridLayout_gaussians->addWidget(chartView_gaussians,0,0);
}
void MainWindow::on_btnInitialize_clicked()
{
    //check it is not empty
    if(inputs.isEmpty())
        return;
    //reset neurons
    radial_neurons.clear();
    //disable k input field
    ui->lineK->setEnabled(false);
    //number of Gaussian neurons
    k=ui->lineK->text().toInt();
    if(k==0){
        k=5;
        ui->lineK->setText("5");
    }

    int numinputs=inputs.size();
    //initialize RBF neurons
    for(int i=1; i<=k; i++)
    {
        int pos=(numinputs/(k+1))*i;
        //qDebug()<<"pos:"<<pos<<"\n";
        radial_neurons.append(new RadialNeuron(inputs.at(pos)));
        //qDebug()<<"Neurona "<<i<<": "<<inputs.at(pos).at(0)<<"-"<<inputs.at(pos).at(1)<<"\n";
    }
    //initialize output linear neuron
    linear_neuron = LinearNeuron(k);
    //series for plotting
    QVector<QLineSeries *> seriesAux;
    //recorre las neuronas
    chart_gaussians->removeAllSeries();
    chart_error->removeSeries(series_error);
    series_error->clear();
    for(int i=0;i<k;i++)
    {
        QLineSeries *serieAux = new QLineSeries();
        //iterate through the inputs for plotting
        for(int j=0;j<numinputs;j++)
        {
            double x=inputs.at(j);
            double y=radial_neurons.at(i)->Gaussian(x);
            serieAux->append(x,y);
            //qDebug()<<"Serie "<<i<<"-X: "<<x<<" Y:"<<y<<"\n";
        }
        seriesAux.append(serieAux);
    }
    //plot initial curves
    int numSeries=seriesAux.size();
    for(int i=0;i<numSeries;i++)
        chart_gaussians->addSeries(seriesAux.at(i));
    chart_gaussians->createDefaultAxes();
}
void MainWindow::ProcessNewFunction(int option)
{
    //clear variables
    Restart();
    //enable k input field
    ui->lineK->setEnabled(true);
    //create a new chart
    QString chart_name="";
    for(double x=-INPUT_RANGE;x<=INPUT_RANGE;x=x+STEP_POINT)
    {
        //asigna y
        double y;
        switch(option){
        case OP_SIN:
            y=qSin(x);
            chart_name="Sine Chart";
            break;
        case OP_COS:
            y=qCos(x);
            chart_name="Cosine Chart";
            break;
        case OP_FUNCTION_3:
            y=((x-2)*(2*x+1))/(1+qPow(x,2));
            chart_name="Chart ((x-2)(2x+1))/(1x^2)";
            break;
        case OP_FUNCTION_4:
            y=2*qSin(x)+qCos(3*x);
            chart_name="Chart 2sin(x)+cos(3x)";
            break;
        case OP_FUNCTION_5:
            y=qSin(2*x)+qLn(qPow(x,2));
            chart_name="Chart sin(2x) + ln(x^2)";
            break;
        }
        series->append(x,y);
        //agrega salidas esperadas
        desired_outputs.append(y);
        //agregar inputs
        inputs.append(x);
    }
    gridLayout->removeWidget(chartView); // remove chart view from the layout
    chart->removeSeries(series); // keep series alive when deleting chart
    delete chart;
    delete chartView;
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(chart_name);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    gridLayout->addWidget(chartView,0,0);
    qApp->processEvents();
}
void MainWindow::on_rbSine_clicked()
{
    if(ui->rbSine->isChecked())
    {
        ProcessNewFunction(OP_SIN);
    }
}

void MainWindow::on_rbCosine_clicked()
{
    if(ui->rbCosine->isChecked())
    {
        ProcessNewFunction(OP_COS);
    }
}

void MainWindow::on_rbFunction3_clicked()
{
    if(ui->rbFunction3->isChecked())
    {
        ProcessNewFunction(OP_FUNCTION_3);
    }
}



void MainWindow::on_rbFunction4_clicked()
{
    if(ui->rbFunction4->isChecked())
    {
        ProcessNewFunction(OP_FUNCTION_4);
    }
}

void MainWindow::on_rbFunction5_clicked()
{
    if(ui->rbFunction5->isChecked())
    {
        ProcessNewFunction(OP_FUNCTION_5);
    }
}
