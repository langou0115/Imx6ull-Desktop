#ifndef GENERAL_H
#define GENERAL_H

#include <QSize>
#include <QString>
#include <linux/videodev2.h>

// dev path
extern QString cpuTempPath;
extern QString ledModePath;
extern QString ledBrightnessPath;
extern QString buzzerPath;
extern const char* cameraPath;

// camera settings
extern int cameraWidth;
extern int cameraHeight;
extern int cameraFormat;

// stretch factor
extern int statusStretch;
extern int bottomStretch;
extern int appstatusStretch;
extern int appBottomStretch;
extern int labelLedStretch;
extern int switchLedStretch;
extern int labelCameraStretch;
extern int switchCamearStretch;

// Weather api
extern QString weatherApi;

// size
extern QSize playerButtonSize;
extern QSize itemSize;
extern QSize windowSize;
extern QSize buttonSize;
extern QSize appSize;
extern QSize widgetSize; // Buzz Weather Video Picture
extern QSize systemInfoWidgetSize; // system information
extern QSize returnButtonSize;
extern QSize returnButtonIconSize;
extern QSize weatherIconSize;

// Img path
extern QString errorPath;
extern QString sunPath;
extern QString cloudsPath;
extern QString rainPath;
extern QString snowPath;
extern QString folderIconPath;
extern QString fileIconPath;
extern QString picImgPath;
extern QString cpuImgPath;
extern QString systemImgPath;
extern QString resoImgPath;
extern QString flashImgPath;
extern QString buttonImgPath;

// style
extern QString winStyle;
extern QString playerStyle;
extern QString fileStyle;
extern QString systemStyle;
extern QString buzzStyle;
extern QString textEditStyle;
extern QString playerButtonOnStyle;
extern QString playerButtonOffStyle;
extern QString returnButtonStyle;

#endif // GENERAL_H
