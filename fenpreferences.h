#ifndef FENPREFERENCES_H
#define FENPREFERENCES_H

#include <QWidget>


namespace Ui {
class FenPreferences;
}

class FenPreferences : public QWidget
{
    Q_OBJECT

public:
    explicit FenPreferences(QWidget *parent = nullptr);
    ~FenPreferences();

signals:
    void confChanged();
public slots:
    void initFen();
    QString getCurrentIdentifier();
    QString getCurrentColor();
    QString getCurrentEmailAddress();


private slots:
    void clickOnValider();
    void clickOnAnnuler();

private:
    Ui::FenPreferences *ui;
    QString mIdentifier;
    QString mColor;
    QString mEmail;




};

#endif // FENPREFERENCES_H
