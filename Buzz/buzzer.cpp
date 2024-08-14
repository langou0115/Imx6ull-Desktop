#include "buzzer.h"
#include "General.h"

#include <QDebug>

Buzzer::Buzzer(QObject *parent)
    : QObject{parent}
{
    buzzerFile = new QFile(buzzerPath);
    if (!buzzerFile->open(QIODevice::WriteOnly | QIODevice::Truncate)){
        buzzerFile = nullptr;
        qDebug() << "open buzzer file error";
    }
}


void Buzzer::setBuzzerOn()
{
    if(buzzerFile){
        buzzerFile->write("1");
        buzzerFile->flush();
    }
}

void Buzzer::setBuzzerOff()
{
    if(buzzerFile){
        buzzerFile->write("0");
        buzzerFile->flush();
    }
}
