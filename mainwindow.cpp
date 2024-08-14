#include "mainwindow.h"
#include "General.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include "SystemInfo/systeminfo.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Window
    ui->verticalLayout->setStretchFactor(ui->Status, statusStretch);
    ui->verticalLayout->setStretchFactor(ui->BottomLayout, bottomStretch);
    setStyleSheet(winStyle);
    setFixedSize(windowSize);
    // status
    setStatus();
    // APP
    setApp();
    // SwtichButton
    setSwitch();
    // Picture
    setPicture();
    // Camera
    setCamera();
    // Weather
    setWeather();
    // LED
    ledThread = new QThread();
    led = new Led();
    led->moveToThread(ledThread);
    ledThread->start();

    connect(switchLed, &SwitchButton::statusChanged, led, &Led::setLedStatus);

    // Buzz
    buzzThread = new QThread();
    buzzer = new Buzzer();
    buzzer->moveToThread(buzzThread);
    buzzThread->start();

    connect(ui->Buzz, &QPushButton::pressed, buzzer, &Buzzer::setBuzzerOn);
    connect(ui->Buzz, &QPushButton::released, buzzer, &Buzzer::setBuzzerOff);

    // Camera
    camera = new Camera(videoshow);
    connect(switchCamera, &SwitchButton::statusChanged, this, [=](bool checked){
        camera->running = checked;
    });
    connect(camera, &Camera::sendExposureParam, this, [=](int min, int max, int default_value){
        qDebug() << "Exposure" << min << max << default_value;
        ui->ExposureSlider->setRange(min, max);
        ui->ExposureSlider->setValue(default_value);
    });

    connect(camera, &Camera::sendGainParam, this, [=](int min, int max, int default_value){
        qDebug() << "Gain" << min << max << default_value;
        ui->GainSlider->setRange(min, max);
        ui->GainSlider->setValue(default_value);
    });

    connect(ui->ExposureSlider, &QSlider::valueChanged, camera, &Camera::setExposure);
    connect(ui->GainSlider, &QSlider::valueChanged, camera, &Camera::setGain);
    camera->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    // status线程退出
    status->quit();
    status->wait();
    status->deleteLater();
    // led线程退出
    ledThread->quit();
    ledThread->wait();
    ledThread->deleteLater();
    // buzzer线程退出
    buzzThread->quit();
    buzzThread->wait();
    buzzThread->deleteLater();
    // camera线程退出
    camera->end = true;
    camera->quit();
    camera->wait();
    camera->deleteLater();
}

void MainWindow::on_System_clicked()
{
    systeminfo->show();
    this->hide();
}


void MainWindow::on_File_clicked()
{
    fileapp->show();
    this->hide();
}

void MainWindow::on_Player_clicked()
{
    // 如果是arm架构，不支持播放视频
    if(QSysInfo::buildCpuArchitecture() == "arm"){
        QMessageBox::warning(this, "警告", "当前设备不支持播放视频！");
        return;
    }
    player->show();
    this->hide();
}

void MainWindow::setStatus()
{
    ui->TimeEdit->setReadOnly(true);
    ui->TimeEdit->setStyleSheet(textEditStyle);
    ui->DateEdit->setReadOnly(true);
    ui->DateEdit->setStyleSheet(textEditStyle);
    ui->CPUTempEdit->setReadOnly(true);
    ui->CPUTempEdit->setStyleSheet(textEditStyle);

    status = new Status();
    status->start();
    connect(status, &Status::updateStatus, this, [=](QString timeStr, QString dateStr, QString cpuTemp){
        ui->TimeEdit->setText(timeStr);
        ui->DateEdit->setText(dateStr);
        ui->CPUTempEdit->setText(cpuTemp);
    });
}

void MainWindow::setApp()
{
    ui->Player->setStyleSheet(playerStyle);
    ui->File->setStyleSheet(fileStyle);
    ui->System->setStyleSheet(systemStyle);
    ui->Buzz->setStyleSheet(buzzStyle);

    ui->Player->setFixedSize(appSize);
    ui->File->setFixedSize(appSize);
    ui->System->setFixedSize(appSize);
    ui->Buzz->setFixedSize(widgetSize);
    ui->Weather->setFixedSize(widgetSize);
    ui->Weather->setReadOnly(true);
    ui->Picture->setFixedSize(widgetSize);

    // File
    fileapp = new FileApp();
    connect(fileapp, &FileApp::showMain, this, [=](){
        this->show();
    });

    // System
    systeminfo = new SystemInfo();
    connect(systeminfo, &SystemInfo::showMain, this, [=](){
        this->show();
    });

    // Player
    player = new Player();
    connect(player, &Player::showMain, this, [=](){
        this->show();
    });

    // Camera
    videoshow = new VideoShow();
    ui->RightLayout->addWidget(videoshow);
}


void MainWindow::setSwitch()
{
    switchLedLayout = new QHBoxLayout();
    switchCameraLayout = new QHBoxLayout();
    labelLed = new QLabel("LED");
    labelLed->setAlignment(Qt::AlignCenter);
    labelCamera = new QLabel("相机");
    labelCamera->setAlignment(Qt::AlignCenter);
    switchLed = new SwitchButton();
    switchCamera = new SwitchButton();
    switchLedLayout->addWidget(labelLed);
    switchLedLayout->addWidget(switchLed);
    switchCameraLayout->addWidget(labelCamera);
    switchCameraLayout->addWidget(switchCamera);
    switchLedLayout->setStretchFactor(labelLed, labelLedStretch);
    switchLedLayout->setStretchFactor(switchLed, switchLedStretch);
    switchCameraLayout->setStretchFactor(labelCamera, labelCameraStretch);
    switchCameraLayout->setStretchFactor(switchCamera, switchCamearStretch);
    ui->Switch->addLayout(switchLedLayout);
    ui->Switch->addLayout(switchCameraLayout);
}

void MainWindow::setWeather()
{
    weather = new Weather();
    // 创建一个QTimer对象
    QTimer *timer = new QTimer(this);

    // 连接定时器的timeout信号到Weather对象的getWeather函数
    connect(timer, &QTimer::timeout, weather, &Weather::getWeather);

    // 设置定时器每隔2小时触发一次
    timer->start(1000 * 60 * 120);
    weather->getWeather();

    connect(weather, &Weather::sendWeather, this, [&](QString weather_, QString temp, QString province, QString city){
        ui->Weather->clear();
        QTextImageFormat imageFormat;
        imageFormat.setWidth(weatherIconSize.width());
        imageFormat.setHeight(weatherIconSize.height());
        QTextCursor cursor(ui->Weather->textCursor());
        if(weather_.contains("晴")){
            imageFormat.setName(sunPath);
        }else if(weather_.contains("雨") && !weather_.contains("雪")){
            imageFormat.setName(rainPath);
        }else if(weather_.contains("雪")){
            imageFormat.setName(snowPath);
        }else{
            imageFormat.setName(cloudsPath);
        }
        cursor.insertImage(imageFormat);
        cursor.insertBlock();
        if(weather_ == "雷阵雨并伴有冰雹") weather_ = "雷阵雨";
        if(weather_ == "大暴雨-特大暴雨") weather_ = "大暴雨";
        cursor.insertText("天气：" + weather_ + "\n");
        cursor.insertText("温度：" + temp + "℃\n");
        cursor.insertText("地点：" + province + city);
    });

    connect(weather, &Weather::getWeatherError, this, [&](){
        ui->Weather->clear();
        QTextCursor cursor(ui->Weather->textCursor());
        QTextImageFormat imageFormat;
        imageFormat.setWidth(weatherIconSize.width());
        imageFormat.setHeight(weatherIconSize.height());
        imageFormat.setName(errorPath);
        cursor.insertImage(imageFormat);
        cursor.insertBlock();
        cursor.insertText("天气获取失败!");
    });
}

void MainWindow::setPicture()
{
    QPixmap picImg(picImgPath);
    QPixmap roundedPixmap(picImg.size());
    roundedPixmap.fill(Qt::transparent); // 设置透明背景
    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform); // 平滑变换
    QPainterPath path;
    path.addRoundedRect(0, 0, picImg.width(), picImg.height(), 20, 20); // 设置圆角半径（20为示例值）
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, picImg);
    ui->Picture->setPixmap(roundedPixmap);
    ui->Picture->setScaledContents(true);
}

void MainWindow::setCamera()
{
    videoshow = new VideoShow();
    QPushButton* Buzz = qobject_cast<QPushButton*>(ui->RightLayout->itemAt(0)->widget());
    ui->RightLayout->removeWidget(Buzz);
    ui->RightLayout->addWidget(videoshow);
    ui->RightLayout->addWidget(Buzz);
    videoshow->setFixedSize(widgetSize);
}

