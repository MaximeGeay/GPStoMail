#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Manzanillo");
    QCoreApplication::setApplicationName("GPStoMail");
    MainWindow w;
    w.show();
    return a.exec();
}
