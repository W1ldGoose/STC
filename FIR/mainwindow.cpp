#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include "fir_filter.h"


MainWindow::MainWindow(QWidget *parent, int length)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    custom_plot = new QCustomPlot();
    // устанавливаем custom_plot в окно приложения
    ui->gridLayout->addWidget(custom_plot,0,0,1,1);

    // Настраиваем шрифт по осям координат
    custom_plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    custom_plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // Делаем видимыми оси X и Y по верхней и правой границам графика,
    // но отключаем на них тики и подписи координат
    custom_plot->xAxis2->setVisible(true);
    custom_plot->yAxis2->setVisible(true);
    custom_plot->xAxis2->setTicks(false);
    custom_plot->yAxis2->setTicks(false);
    custom_plot->xAxis2->setTickLabels(false);
    custom_plot->yAxis2->setTickLabels(false);
    custom_plot->legend->setVisible(true);

    // инициализируем график и привязываем его к осям
    graphic_input = new QCPGraph(custom_plot->xAxis, custom_plot->yAxis);
    graphic_input->setName("Input signal");

    graphic_output = new QCPGraph(custom_plot->xAxis, custom_plot->yAxis);
    graphic_output->setName("Filtred output");

    custom_plot->graph(0)->setPen(QPen(Qt::blue));
    custom_plot->graph(1)->setPen(QPen(Qt::red));


    int arr_size = 1000;
    int samp_freq = 1000;
    int filter_length = 400;
    auto [time_stamps, input] = generate_in_signal(arr_size);

    float *output = FIR_filter(input, arr_size, samp_freq, length);

    float *output2 = FIR_filter_SIMD(input, arr_size, samp_freq, length);

    std::vector<double> time(time_stamps,time_stamps+arr_size);
    std::vector<double> in(input,input+arr_size);
    std::vector<double> out(output,output+arr_size);
    std::vector<double> out2(output2,output2+arr_size);

    custom_plot->graph(0)->setData(QVector<double>::fromStdVector(time),QVector<double>::fromStdVector(in));
     custom_plot->graph(1)->setData(QVector<double>::fromStdVector(time),QVector<double>::fromStdVector(out2));


    custom_plot->rescaleAxes();
    custom_plot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

