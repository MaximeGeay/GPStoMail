#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>


#include "mainwindow.h"
#include "ui_mainwindow.h"

#define version "GPStoMail 0.1"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(version);
    mFenPref=new FenPreferences();
    mNavData=new NavData();

    mLayoutDataManager=new QHBoxLayout();
    mLayoutDataManager->setAlignment(Qt::AlignTop);
    mLayoutDataManager->setDirection(QBoxLayout::LeftToRight);
    mLayoutDataManager->addWidget(mNavData);
    ui->gb_Datas->setLayout(mLayoutDataManager);

    QObject::connect(ui->actionPreferences,&QAction::triggered,mFenPref,&FenPreferences::initFen);
    QObject::connect(ui->actionQuitter,&QAction::triggered,this,&MainWindow::close);
    QObject::connect(mFenPref,&FenPreferences::confChanged,mNavData,&NavData::initCom);
    QObject::connect(mNavData,&NavData::errorString,this,&MainWindow::readError);
    QObject::connect(ui->btn_NewMail,&QPushButton::clicked,this,&MainWindow::clickOnNewMail);
    QObject::connect(ui->btn_actualiser,&QPushButton::clicked,this,&MainWindow::clickOnActualise);
    QObject::connect(ui->btn_Generate,&QPushButton::clicked,this,&MainWindow::clickOnGenerate);
    QObject::connect(ui->btn_Cancel,&QPushButton::clicked,this,&MainWindow::clickOnCancel);

    mNavData->initCom();
    ui->sw_Principal->setCurrentIndex(0);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readError(QString sError)
{
    ui->statusbar->showMessage(sError,3000);
    qDebug()<<sError;
}

void MainWindow::majData()
{
    NavData::stGPSDatas currentGps=mNavData->getLastGPS();

    ui->sp_COG->setValue(currentGps.COG);
    ui->sp_SOG->setValue(currentGps.SOG);
    ui->le_Position->setText(currentGps.position.toString(QGeoCoordinate::DegreesMinutesWithHemisphere));

}

void MainWindow::clickOnNewMail()
{
    majData();
    ui->sw_Principal->setCurrentIndex(1);


}

void MainWindow::clickOnActualise()
{
    majData();

}

void MainWindow::clickOnGenerate()
{
    NavData::stGPSDatas currentGps;
    currentGps.COG=ui->sp_COG->value();
    currentGps.SOG=ui->sp_SOG->value();

    currentGps.position=stringToPos(ui->le_Position->text());
    if(!currentGps.position.isValid())
    {
        QMessageBox::warning(this,"Position erroné","Le format de position n'est pas correct");
        return;
    }

    QString sLatitude=QString::number(currentGps.position.latitude(),'f',6);
    QString sLongitude=QString::number(currentGps.position.longitude(),'f',6);
    QString sId=mFenPref->getCurrentIdentifier();
    QString sColor=mFenPref->getCurrentColor();
    QString sComment=QString("Vitesse:%1nd,route:%2°,%3").arg(QString::number(currentGps.SOG,'f',1)).arg(QString::number(currentGps.COG,'f',1)).arg(ui->le_Comments->text());
    sComment.replace(" ","%20");

    QString sMessage=QString("**SMS*%1*%2*%3*%4*%5").arg(sLatitude,sLongitude,sId,sColor,sComment);

    QString sEmail=mFenPref->getCurrentEmailAddress();

    QString sUrl=QString("mailto:%1?subject=ManzanilloPosition&body=%2").arg(sEmail,sMessage);
    qDebug()<<sUrl;

    QDesktopServices::openUrl(QUrl(sUrl,QUrl::TolerantMode));





    clickOnCancel();
}

void MainWindow::clickOnCancel()
{
    ui->sw_Principal->setCurrentIndex(0);
}

QGeoCoordinate MainWindow::stringToPos(QString sPos)
{

    //47° 37.264' N, 1° 18.254' E
    QString sLat=sPos.section(",",0,0);
    QString sLong=sPos.section(",",1,1);
    QGeoCoordinate geo(latMinToDec(sLat),longMinToDec(sLong));
    return geo;
}

double MainWindow::latMinToDec(QString sLatitude)
{
    //format : DD° MM.ddd' H
    //         47° 37.264' N
        int nDeg=sLatitude.section("°",0,0).toInt();
        double dMin=sLatitude.section("°",1,1).section("'",0,0).remove(" ").toDouble();
        QString cHemi=sLatitude.section("'",1,1).remove(" ");

        double dLatDec=nDeg+(dMin/60);
        if(cHemi=="S")
            dLatDec=-dLatDec;

        return dLatDec;
}

double MainWindow::longMinToDec(QString sLongitude)
{
    //format :DD° MM.ddd' H
    //        1° 18.254' E
    int nDeg=sLongitude.section("°",0,0).toInt();
    double dMin=sLongitude.section("°",1,1).section("'",0,0).remove(" ").toDouble();
    QString cHemi=sLongitude.section("'",1,1).remove(" ");

    double dLongDec=nDeg+(dMin/60);
    if(cHemi=="W")
        dLongDec=-dLongDec;

    return dLongDec;
}


