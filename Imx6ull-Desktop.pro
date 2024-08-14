QT       += core gui multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Buzz/buzzer.cpp \
    Camera/camera.cpp \
    Camera/videoshow.cpp \
    FileApp/fileapp.cpp \
    General.cpp \
    Led/led.cpp \
    Status/status.cpp \
    Weather/weather.cpp \
    main.cpp \
    mainwindow.cpp \
    SwitchButton/SwitchButton.cpp \
    SystemInfo/systeminfo.cpp \
    Player/player.cpp

HEADERS += \
    Buzz/buzzer.h \
    Camera/camera.h \
    Camera/videoshow.h \
    FileApp/fileapp.h \
    General.h \
    Led/led.h \
    Status/status.h \
    Weather/weather.h \
    mainwindow.h \
    SwitchButton/SwitchButton.h \
    SystemInfo/systeminfo.h \
    Player/player.h

FORMS += \
    FileApp/fileapp.ui \
    mainwindow.ui \
    SystemInfo/systeminfo.ui \
    Player/player.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
