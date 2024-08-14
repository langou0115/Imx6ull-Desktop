#include "General.h"
#include <linux/videodev2.h>


// dev path
QString cpuTempPath = "/sys/class/thermal/thermal_zone0/temp";
QString ledModePath = "/sys/class/leds/sys-led/trigger";
QString ledBrightnessPath = "/sys/class/leds/sys-led/brightness";
QString buzzerPath = "/sys/class/leds/beep/brightness";
const char* cameraPath = "/dev/video2";

// camera settings
int cameraWidth = 640;
int cameraHeight = 480;
int cameraFormat = V4L2_PIX_FMT_YUYV;


// stretch factor
int statusStretch = 2;
int bottomStretch = 10;
int appstatusStretch = 1;
int appBottomStretch = 11;
int labelLedStretch = 4;
int switchLedStretch = 3;
int labelCameraStretch = 4;
int switchCamearStretch = 3;

// Weather api
QString weatherApi =
    "your api";

// size
QSize playerButtonSize(50, 50);
QSize itemSize(100, 50);
QSize windowSize(800, 480);
QSize buttonSize(100, 100);
QSize appSize(100, 100);
QSize widgetSize(200, 200); // Buzz Weather Video Picture
QSize systemInfoWidgetSize(160, 160); // system information
QSize returnButtonSize(100, 60);
QSize returnButtonIconSize(120, 60);
QSize weatherIconSize(100, 100);

// Img path
QString errorPath = ":/Img/error.png";
QString sunPath = ":/Img/sun.png";
QString cloudsPath = ":/Img/clouds.png";
QString rainPath = ":/Img/rain.png";
QString snowPath = ":/Img/snow.png";
QString folderIconPath = ":/Img/dir.png";
QString fileIconPath = ":/Img/file.png";
QString picImgPath = ":/Img/bg.png";
QString cpuImgPath = ":/Img/cpu.png";
QString systemImgPath = ":/Img/os.png";
QString resoImgPath = ":/Img/resolution.png";
QString flashImgPath = ":/Img/rom.png";
QString buttonImgPath = ":/Img/leftArrow.png";

// style
QString winStyle = "background-color: rgb(230,230,250);";
QString playerStyle = "QPushButton {"
                    "border-radius: 10px;"
                    "background-image: url(:/Img/player.png);"
                    "background-color: lightblue;"
                    "background-position: center;"
                    "background-repeat: no-repeat;"
                    "outline: none;"
                    "}";
QString fileStyle = "QPushButton {"
                    "border-radius: 10px;"
                    "background-image: url(:/Img/fileapp.png);"
                    "background-color: lightblue;"
                    "background-position: center;"
                    "background-repeat: no-repeat;"
                    "outline: none;"
                    "}";
QString systemStyle = "QPushButton {"
                      "border-radius: 10px;"
                      "background-image: url(:/Img/linux.png);"
                      "background-color: lightblue;"
                      "background-position: center;"
                      "background-repeat: no-repeat;"
                      "outline: none;"
                      "}";
QString buzzStyle = "QPushButton {"
                    "border: none;"
                    "background-image: url(:/Img/buzz.png);"
                    "background-position: center;"
                    "background-repeat: no-repeat;"
                    "outline: none;"
                    "}";
QString playerButtonOnStyle = "QPushButton {"
                              "border: none;"
                              "background-image: url(:/Img/play.png);"
                              "background-position: center;"
                              "background-repeat: no-repeat;"
                              "outline: none;"
                              "}";
QString playerButtonOffStyle = "QPushButton {"
                               "border: none;"
                               "background-image: url(:/Img/pause.png);"
                               "background-position: center;"
                               "background-repeat: no-repeat;"
                               "outline: none;"
                               "}";
QString returnButtonStyle = "QPushButton {"
                            "outline: none;"
                            "}";
QString textEditStyle = "QLineEdit {"
                    "border: none; /* 移除边框 */"
                    "}";
