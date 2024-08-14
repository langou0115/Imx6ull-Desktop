#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <Status/status.h>
#include <FileApp/fileapp.h>
#include <Weather/weather.h>
#include <Player/player.h>
#include <Led/led.h>
#include <Buzz/buzzer.h>
#include <Camera/videoshow.h>
#include "Camera/camera.h"
#include "SystemInfo/systeminfo.h"
#include "SwitchButton/SwitchButton.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_System_clicked();

    void on_File_clicked();

    void on_Player_clicked();

private:
    void setStatus();
    void setApp();
    void setSwitch();
    void setWeather();
    void setPicture();
    void setCamera();
    Ui::MainWindow *ui;
    QLabel *labelLed;
    QLabel *labelCamera;
    QHBoxLayout *switchLedLayout;
    QHBoxLayout *switchCameraLayout;
    SwitchButton *switchLed;
    SwitchButton *switchCamera;
    VideoShow *videoshow;
    Status *status;
    SystemInfo *systeminfo;
    FileApp *fileapp;
    Weather *weather;
    Player *player;
    Led *led;
    Buzzer *buzzer;
    Camera *camera;

    QThread *ledThread;
    QThread *buzzThread;
};
#endif // MAINWINDOW_H
