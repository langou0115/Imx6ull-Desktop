#include "status.h"
#include "General.h"

#include <QTime>
#include <QThread>
#include <QDebug>

Status::Status(QObject *parent)
    : QThread(parent)
{
    cputemp = new QFile(cpuTempPath);
    if(!cputemp->open(QIODevice::ReadOnly)){
        cputemp = nullptr;
        qDebug() << "Open file failed!";
    }
}

void Status::run()
{
    timer = new QTimer();
    updateDateTime();
    connect(timer, &QTimer::timeout, this, [=](){
        updateDateTime();
    });
    timer->start(1000);
    exec();
}

void Status::updateDateTime()
{
    QTime time = QTime::currentTime();
    QString timeStr = time.toString("hh:mm:ss");
    QString dateStr = QDate::currentDate().toString("yyyy-MM-dd");
    QString cpuTemp = "N/A";
    if(cputemp != nullptr){
        cputemp->seek(0);
        cpuTemp = QString::number(cputemp->readAll().toInt() / 1000.0, 'f', 1) + "°C";
    }
    emit updateStatus(timeStr, dateStr, cpuTemp);
}

