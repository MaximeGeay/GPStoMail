#include <QSettings>
#include <QDebug>
#include "navdata.h"
#include "ui_navdata.h"

NavData::NavData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavData)
{
    ui->setupUi(this);
    mUdpGPS=new UDPData();
    mTimerTime=new QTimer;

    mUdpGPS->setInterval(3000);

    mDateHeurePrec=QDateTime::currentDateTimeUtc();

    //initCom();
    QObject::connect(mUdpGPS,&UDPData::dataReceived,this,&NavData::readData);
    QObject::connect(mUdpGPS,&UDPData::timeout,this,&NavData::gpsTimeout);
    QObject::connect(mUdpGPS,&UDPData::errorString,this,&NavData::udpError);


    QObject::connect(mTimerTime,&QTimer::timeout,this,&NavData::majHeure);
    mTimerTime->start(100);
}

NavData::~NavData()
{
    delete mUdpGPS;
    delete ui;
}

void NavData::initCom()
{


    QSettings settings;
    if(mUdpGPS->UdpPort()!=0)
        mUdpGPS->close();




    mUdpGPS->initCom(settings.value("PortGPS",10110).toInt());

}

NavData::stGPSDatas NavData::getLastGPS()
{
    return mLastGPS;
}

void NavData::readData(QString sTrame)
{
     stGPSDatas uneData;

        if(sTrame.section(",",0,0)=="$INRMC" || sTrame.section(",",0,0)=="$GPRMC")
        {
            QString sDateHeure=sTrame.section(",",1,1).section(".",0,0)+" "+sTrame.section(",",9,9);
            QString sLat=sTrame.section(",",3,3);
            QString sLatDeg= sLat.left(2);
            QString sLatMin=sLat.remove(0,2);
            QString sLatHemi=sTrame.section(",",4,4);
            QString sLatitude=QString("%1,%2,%3").arg(sLatHemi).arg(sLatDeg).arg(sLatMin);
            QString sLong=sTrame.section(",",5,5);
            QString sLongDeg= sLong.left(3);
            QString sLongMin=sLong.remove(0,3);
            QString sLongHemi=sTrame.section(",",6,6);
            QString sLongitude=QString("%1,%2,%3").arg(sLongHemi).arg(sLongDeg).arg(sLongMin);
            QString sSOG=sTrame.section(",",7,7);
            QString sCOG=sTrame.section(",",8,8);
            uneData.dateheure=QDateTime::fromString(sDateHeure,"hhmmss ddMMyy");
            uneData.dateheure=uneData.dateheure.addYears(100);
            uneData.dateheure.setTimeSpec(Qt::UTC);
            uneData.position.setLatitude(latMinToDec(sLatitude));
            uneData.position.setLongitude(longMinToDec(sLongitude));
            uneData.SOG=sSOG.toDouble();
            uneData.COG=sCOG.toDouble();
            uneData.dispo=true;

            ui->l_DateHeureGPS->setText(QString("Heure GPS : %1").arg(uneData.dateheure.toString("dd/MM/yyyy hh:mm:ss")));
            ui->l_Position->setText(QString("Position: %1").arg(uneData.position.toString(QGeoCoordinate::DegreesMinutesWithHemisphere)));
            ui->l_COG->setText(QString("Route fond: %1°").arg(QString::number(uneData.COG,'f',0)));
            ui->l_SOG->setText(QString("Vitesse fond: %1 nds").arg(QString::number(uneData.SOG,'f',1)));

            mLastGPS=uneData;
            emit gpsReceived(uneData);



        }







}



void NavData::gpsTimeout()
{
    ui->l_DateHeureGPS->setText("Données GPS non reçues");
    ui->l_Position->setText(QString("Position:"));
    ui->l_COG->setText(QString("Route fond:"));
    ui->l_SOG->setText(QString("Vitesse fond:"));
    mLastGPS.dispo=false;
}

void NavData::majHeure()
{
    QDateTime currentTime=QDateTime::currentDateTimeUtc();
    if(currentTime.time().second()!=mDateHeurePrec.time().second())
    {
        mDateHeurePrec=currentTime;
        ui->l_dtPC->setText(QString("Heure PC: %1").arg(currentTime.toString("dd/MM/yyyy hh:mm:ss")));
    }
}

void NavData::udpError(QString sMsg)
{
    emit errorString(sMsg);
}

double NavData::latMinToDec(QString sLatitude)
{
    //format : H,DD,MM.ddddd,
        //         S,12,30.54443
        int nDeg=sLatitude.section(",",1,1).toInt();
        double dMin=sLatitude.section(",",2,2).toDouble();
        QString cHemi=sLatitude.section(",",0,0);

        double dLatDec=nDeg+(dMin/60);
        if(cHemi=="S")
            dLatDec=-dLatDec;

        return dLatDec;

}


double NavData::longMinToDec(QString sLongitude)
{
    //format :H,DDD,MM.ddddd
    //        E,044,55.20103
    int nDeg=sLongitude.section(",",1,1).toInt();
    double dMin=sLongitude.section(",",2,2).toDouble();
    QString cHemi=sLongitude.section(",",0,0);

    double dLongDec=nDeg+(dMin/60);
    if(cHemi=="W")
        dLongDec=-dLongDec;

    return dLongDec;
}
