#ifndef CAMERA_H
#define CAMERA_H

#include "Camera/videoshow.h"
#include "General.h"
#include <QObject>
#include <QThread>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

class Camera : public QThread
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);
    ~Camera();
    void run() override;
    bool running, end = false, firstSetting = true;
    void getControlRange(int control_id, int *min, int *max, int *default_value);
    void setExposure(int exposure);
    void setGain(int value);
signals:
    void sendExposureParam(int min, int max, int default_value);
    void sendGainParam(int min, int max, int default_value);


private:
    VideoShow *parent;
    inline void yuv_to_rgb(unsigned char *yuv, unsigned char *rgb);
    struct cam_buf_info {
        void *start;      //帧缓冲起始地址
        unsigned long length;       //帧缓冲长度
    };
    static const constexpr int FRAMEBUFFER_COUNT = 1;
    cam_buf_info buffers[FRAMEBUFFER_COUNT];
    int camerafd, ret;
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers reqbuf;
    struct v4l2_buffer buf;
    struct v4l2_control control;
    struct v4l2_queryctrl queryctrl;
    enum v4l2_buf_type type;
    fd_set fds;
    struct timeval tv;
    bool iscapture;
    void handleData(unsigned char *bufData);

    void startCapture();
    void stopCapture();
};

#endif // CAMERA_H
