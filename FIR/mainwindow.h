#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int length;

public:
    explicit MainWindow(QWidget *parent = nullptr, int length = 400);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // объявляем объект QCustomPlot
    QCustomPlot *custom_plot;
    // график 1
    QCPGraph *graphic_input;
    // график 2
    QCPGraph *graphic_output;

};

#endif // MAINWINDOW_H
