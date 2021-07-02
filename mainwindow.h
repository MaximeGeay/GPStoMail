#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "navdata.h"
#include "fenpreferences.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readError(QString sError);
    void majData();
    void clickOnNewMail();
    void clickOnActualise();
    void clickOnGenerate();
    void clickOnCancel();


private:
    Ui::MainWindow *ui;
    NavData* mNavData;
    QHBoxLayout *mLayoutDataManager;
    FenPreferences *mFenPref;

    QGeoCoordinate stringToPos(QString sPos);
    double latMinToDec(QString sLatitude);
    double longMinToDec(QString sLongitude);


};
#endif // MAINWINDOW_H
