#include "systeminfo.h"
#include "General.h"
#include "ui_systeminfo.h"
#include <QDebug>

SystemInfo::SystemInfo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SystemInfo)
{
    ui->setupUi(this);
    setFixedSize(windowSize);
    setStyleSheet(winStyle);
    ui->verticalLayout->setStretchFactor(ui->toplayout, appstatusStretch);
    ui->verticalLayout->setStretchFactor(ui->bottomlayout, appBottomStretch);

    QPixmap buttonImg(buttonImgPath);
    ui->ReturnButton->setIcon(buttonImg);
    ui->ReturnButton->setIconSize(returnButtonIconSize);
    ui->ReturnButton->setFixedSize(returnButtonSize);
    ui->ReturnButton->setStyleSheet(returnButtonStyle);

    ui->cpuImg->setFixedSize(systemInfoWidgetSize);
    QPixmap cpuImg(cpuImgPath);
    ui->cpuImg->setPixmap(cpuImg);
    ui->cpuImg->setScaledContents(true);

    struct utsname buffer;
    if (uname(&buffer) != 0) {
        perror("uname");
        QApplication::exit(1);
    }
    // systemInfo = buffer.sysname + buffer.release
    QString release = QString(buffer.release).split('-').first();
    QString systemInfo = QString("%1 %2").arg(buffer.sysname).arg(release);
    ui->systemInfo->setText(systemInfo);
    ui->systemImg->setFixedSize(systemInfoWidgetSize);
    QPixmap systemImg(systemImgPath);
    ui->systemImg->setPixmap(systemImg);
    ui->systemImg->setScaledContents(true);

    ui->resoImg->setFixedSize(systemInfoWidgetSize);
    QPixmap resoImg(resoImgPath);
    ui->resoImg->setPixmap(resoImg);
    ui->resoImg->setScaledContents(true);

    ui->flashImg->setFixedSize(systemInfoWidgetSize);
    QPixmap flashImg(flashImgPath);
    ui->flashImg->setPixmap(flashImg);
    ui->flashImg->setScaledContents(true);

}

SystemInfo::~SystemInfo()
{
    delete ui;
}

void SystemInfo::on_ReturnButton_clicked()
{
    this->close();
    emit showMain();
}

