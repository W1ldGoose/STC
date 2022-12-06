#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(nullptr, QCoreApplication::arguments().at(1).toInt());
    w.show();
    return a.exec();
}
