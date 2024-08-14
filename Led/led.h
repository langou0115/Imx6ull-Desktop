#ifndef LED_H
#define LED_H

#include <QFile>
#include <QObject>

class Led : public QObject
{
    Q_OBJECT
public:
    explicit Led(QObject *parent = nullptr);
    void initLed();
public slots:
    void setLedStatus(bool checked);
signals:

private:
    QFile *ledBrightness;
};

#endif // LED_H
