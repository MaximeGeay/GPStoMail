#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>

#include "fenpreferences.h"
#include "ui_fenpreferences.h"


FenPreferences::FenPreferences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FenPreferences)
{
    ui->setupUi(this);

    QObject::connect(ui->btn_Valider,&QPushButton::clicked,this,&FenPreferences::clickOnValider);
    QObject::connect(ui->btn_Annuler,&QPushButton::clicked,this,&FenPreferences::clickOnAnnuler);

    QSettings settings;

    mEmail=settings.value("Email","address@provider.com").toString();
    mIdentifier=settings.value("Identifier","Boat").toString();
    mColor=settings.value("Color","red").toString();

}

FenPreferences::~FenPreferences()
{
    delete ui;
}

void FenPreferences::initFen()
{
    QSettings settings;
    int nPortGps=settings.value("PortGPS",10110).toInt();
     ui->sp_PortGPS->setValue(nPortGps);

    ui->le_Id->setText(mIdentifier);
    ui->le_Email->setText(mEmail);
    ui->cb_Color->setCurrentIndex(ui->cb_Color->findText(mColor));


    this->show();
}

QString FenPreferences::getCurrentIdentifier()
{
    return mIdentifier;
}

QString FenPreferences::getCurrentColor()
{
    return mColor;
}

QString FenPreferences::getCurrentEmailAddress()
{
    return mEmail;
}


void FenPreferences::clickOnValider()
{
    QSettings settings;
    settings.setValue("PortGPS",ui->sp_PortGPS->value());

    mEmail=ui->le_Email->text();
    mIdentifier=ui->le_Id->text();
    mColor=ui->cb_Color->currentText();
    settings.setValue("Email",mEmail);
    settings.setValue("Identifier",mIdentifier);
    settings.setValue("Color",mColor);


    emit confChanged();
    clickOnAnnuler();

}

void FenPreferences::clickOnAnnuler()
{
    this->close();
}





