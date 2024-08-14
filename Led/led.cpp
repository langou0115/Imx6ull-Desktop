#include "led.h"
#include "General.h"

#include <QDebug>

Led::Led(QObject *parent)
    : QObject{parent}
{
    QFile ledMode(ledModePath);
    if (ledMode.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        ledMode.write("none");
    }else{
        qDebug() << "open led mode error";
    }

    ledBrightness = new QFile(ledBrightnessPath);
    if (!ledBrightness->open(QIODevice::WriteOnly | QIODevice::Truncate)){
        ledBrightness = nullptr;
        qDebug() << "open led file error";
    }
}

void Led::setLedStatus(bool checked)
{
    if(checked){
        if(ledBrightness){
            ledBrightness->write("1");
            ledBrightness->flush();
        }
    }else{
        if(ledBrightness){
            ledBrightness->write("0");
            ledBrightness->flush();
        }
    }
}
