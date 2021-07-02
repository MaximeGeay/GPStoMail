#ifndef NAVDATA_H
#define NAVDATA_H

#include <QWidget>
#include <QGeoCoordinate>
#include <QDateTime>
#include <QTimer>
#include "udpdata.h"

namespace Ui {
class NavData;
}

class NavData : public QWidget
{
    Q_OBJECT

public:

    struct stGPSDatas{
        QGeoCoordinate position;
        double SOG=0,COG=0;
        QDateTime dateheure;
        bool dispo=false;
    };


    explicit NavData(QWidget *parent = nullptr);
    ~NavData();



signals:
    void gpsReceived(stGPSDatas);
    void errorString(QString);

public slots:
    void initCom();
    stGPSDatas getLastGPS();

private slots:
     void readData(QString sTrame);
     void gpsTimeout();
     void majHeure();
     void udpError(QString sMsg);

private:
    Ui::NavData *ui;

    UDPData *mUdpGPS;
    double latMinToDec(QString sLatitude);
    double longMinToDec(QString sLongitude);

    QTimer *mTimerTime;
    stGPSDatas mLastGPS;

    QDateTime mDateHeurePrec;
};

#endif // NAVDATA_H
