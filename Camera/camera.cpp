#include "camera.h"
#include "Camera/videoshow.h"
#include "General.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QThread>
#include <QString>
#include <unistd.h>


Camera::Camera(QObject *parent)
    : QThread{parent}
{
    this->parent = (VideoShow *) parent;
    running = false;
    iscapture = false;
    /* 打开设备 */
    camerafd = -1;
    camerafd = open(cameraPath, O_RDWR | O_NONBLOCK, 0);
    if(camerafd < 0){
        qDebug("open camera failed");
        return;
    }
    // 过于玄学....难评.....
    CLEAR(control);
    control.id = V4L2_CID_EXPOSURE_AUTO;
    control.value = V4L2_EXPOSURE_MANUAL; // 手动模式

    if (ioctl(camerafd, VIDIOC_S_CTRL, &control) < 0) {
        qDebug("Setting exposure auto failed");
        return;
    }
}

Camera::~Camera()
{
    if(iscapture) stopCapture();
    for(int i = 0; i < FRAMEBUFFER_COUNT; i++){
        munmap(buffers[i].start,buffers[i].length);
    }
    close(camerafd);
}

void Camera::run()
{
    /* 获取摄像头能力 */
    ioctl(camerafd, VIDIOC_QUERYCAP, &cap);
    /* 判断是否是视频采集设备 */
    if (!(V4L2_CAP_VIDEO_CAPTURE & cap.capabilities)) {
        qDebug("Error:No capture video device!");
        return;
    }
    /* 设置视频格式 */
    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = cameraWidth;
    fmt.fmt.pix.height = cameraHeight;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    if (0 > ioctl(camerafd, VIDIOC_S_FMT, &fmt)) {
        qDebug("set format failed");
        return;
    }
    qDebug("set format success");
    /* 判断是否已经设置为我们要求的YUYV像素格式,否则表示该设备不支持YUYV像素格式 */
    if (V4L2_PIX_FMT_YUYV != fmt.fmt.pix.pixelformat) {
        qDebug("Error: the device does not support YUYV format!");
        return;
    }
    /* 获取实际的帧宽高度 */
    qDebug("当前视频帧大小<%d * %d>, 颜色空间:%d", fmt.fmt.pix.width, fmt.fmt.pix.height,fmt.fmt.pix.colorspace);

    if(firstSetting){
        firstSetting = false;
        int exposureMin = 0, exposureMax = 100, exposure_default_value = 50;
        getControlRange(V4L2_CID_EXPOSURE_ABSOLUTE, &exposureMin, &exposureMax, &exposure_default_value);
        emit sendExposureParam(exposureMin, exposureMax, exposure_default_value);
        qDebug("exposureMin:%d, exposureMax:%d, exposure_default_value:%d", exposureMin, exposureMax, exposure_default_value);
        int gainMin = 0, gainMax = 100, gain_default_value = 50;
        getControlRange(V4L2_CID_GAIN, &gainMin, &gainMax, &gain_default_value);
        qDebug("gainMin:%d, gainMax:%d, gain_default_value:%d", gainMin, gainMax, gain_default_value);
        emit sendGainParam(gainMin, gainMax, gain_default_value);
    }

    /* 设置帧缓冲数量 */
    reqbuf.count = FRAMEBUFFER_COUNT;       //帧缓冲的数量
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    if (0 > ioctl(camerafd, VIDIOC_REQBUFS, &reqbuf)) {
        qDebug("request buffer failed");
        return;
    }
    qDebug("request buffer success");

    /* 建立内存映射 */
    for(int i = 0; i < FRAMEBUFFER_COUNT; i++){
        CLEAR(buf);
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if(0 > ioctl(camerafd, VIDIOC_QUERYBUF, &buf)){
            qDebug("VIDIOC_QUERYBUF failed");
            return;
        }

        buffers[i].length = buf.length;
        buffers[i].start = mmap(NULL,buf.length,PROT_READ | PROT_WRITE, MAP_SHARED, camerafd, buf.m.offset);
        if (MAP_FAILED == buffers[i].start) {
            qDebug("mmap error");
            return;
        }
    }
    qDebug("memory map success");
    /* 获取帧数据 */
    while(true){
        if(end) break;
        if (!running) {
            if(iscapture){
                /* 关闭视频流 */
                iscapture = false;
                stopCapture();
            }
            msleep(100); // 线程处于等待状态，减少CPU占用
            continue;
        }else{
            if(!iscapture){
                /* 开启视频流 */
                iscapture = true;
                startCapture();
            }
        }
        do{
            /* 初始化select()来进行I/O端口复用 */
            FD_ZERO(&fds);
            FD_SET(camerafd,&fds);

            /* TimeOut */
            tv.tv_sec  = 2;
            tv.tv_usec = 0;

            ret = select(camerafd+1,&fds,NULL,NULL,&tv);
        } while ((ret == -1) && (errno = EINTR));

        if(ret == -1){
            qDebug("select I/O failed");
            return;
        }
        qDebug("select I/O success");
        //帧数据处理
        unsigned char onebuf[cameraWidth*cameraHeight*3];
        handleData(onebuf);
        QImage img_stream = QImage(onebuf,cameraWidth,cameraHeight,QImage::Format_RGB888);
        img_stream = img_stream.scaled(widgetSize, Qt::KeepAspectRatio);
        if(parent->isVisible()){
            parent->img = img_stream;
            parent->update();
        }
    }
}

void Camera::getControlRange(int control_id, int *min, int *max, int *default_value)
{
    CLEAR(queryctrl);
    queryctrl.id = control_id;
    if (-1 == ioctl(camerafd, VIDIOC_QUERYCTRL, &queryctrl)) {
        perror("VIDIOC_QUERYCTRL");
        return;
    }
    if (queryctrl.minimum < queryctrl.maximum) {
        *min = queryctrl.minimum;
        *max = queryctrl.maximum;
        *default_value = queryctrl.default_value;
    }
}

void Camera::setExposure(int exposure)
{
    CLEAR(control);
    control.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    control.value = exposure;  // 曝光值

    if (ioctl(camerafd, VIDIOC_S_CTRL, &control) < 0) {
        perror("Setting exposure failed");
        return;
    }
}

void Camera::setGain(int gain)
{
    CLEAR(control);
    control.id = V4L2_CID_GAIN;
    control.value = gain;  // 增益值

    if (ioctl(camerafd, VIDIOC_S_CTRL, &control) < 0) {
        perror("Setting gain failed");
    }
}

void Camera::handleData(unsigned char *bufData)
{
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    /* 出队 */
    if(0 > ioctl(camerafd,VIDIOC_DQBUF,&buf)){
        qDebug("出队失败");
        return;
    }
    qDebug("出队成功");

    //数据处理
    unsigned char temp[buf.bytesused];
    memcpy(temp, buffers[buf.index].start, buf.bytesused);
    yuv_to_rgb(temp, bufData);

    /* 再次入队*/
    if (0 > ioctl(camerafd, VIDIOC_QBUF, &buf)) {
        qDebug("入队失败");
        return;
    }
    qDebug("入队成功");
}

void Camera::startCapture()
{
    /* 入队 */
    for(unsigned int i = 0; i < FRAMEBUFFER_COUNT; i++){
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (0 > ioctl(camerafd, VIDIOC_QBUF, &buf)) {
            qDebug("入队失败");
            return;
        }
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 > ioctl(camerafd, VIDIOC_STREAMON, &type)) {
        qDebug("open stream failed");
        return;
    }
    qDebug("open stream success");
}

void Camera::stopCapture()
{
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 > ioctl(camerafd, VIDIOC_STREAMOFF, &type)) {
        qDebug("close stream failed");
        return;
    }
    qDebug("close stream success");
    msleep(1000);
}

void Camera::yuv_to_rgb(unsigned char *yuv, unsigned char *rgb)
{
    unsigned char* y0 = yuv + 0;
    unsigned char* u0 = yuv + 1;
    unsigned char* y1 = yuv + 2;
    unsigned char* v0 = yuv + 3;

    unsigned  char* r0 = rgb + 0;
    unsigned  char* g0 = rgb + 1;
    unsigned  char* b0 = rgb + 2;
    unsigned  char* r1 = rgb + 3;
    unsigned  char* g1 = rgb + 4;
    unsigned  char* b1 = rgb + 5;

    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    for(int i = 0; i <= (cameraWidth * cameraHeight) / 2 ;i++)
    {
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128);
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128);

        bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128);
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128);


        if(rt0 > 250)      rt0 = 255;
        if(rt0< 0)        rt0 = 0;

        if(gt0 > 250)     gt0 = 255;
        if(gt0 < 0)    gt0 = 0;

        if(bt0 > 250)    bt0 = 255;
        if(bt0 < 0)    bt0 = 0;

        if(rt1 > 250)    rt1 = 255;
        if(rt1 < 0)    rt1 = 0;

        if(gt1 > 250)    gt1 = 255;
        if(gt1 < 0)    gt1 = 0;

        if(bt1 > 250)    bt1 = 255;
        if(bt1 < 0)    bt1 = 0;

        *r0 = (unsigned char)rt0;
        *g0 = (unsigned char)gt0;
        *b0 = (unsigned char)bt0;

        *r1 = (unsigned char)rt1;
        *g1 = (unsigned char)gt1;
        *b1 = (unsigned char)bt1;

        yuv = yuv + 4;
        rgb = rgb + 6;
        if(yuv == NULL)
            break;

        y0 = yuv;
        u0 = yuv + 1;
        y1 = yuv + 2;
        v0 = yuv + 3;

        r0 = rgb + 0;
        g0 = rgb + 1;
        b0 = rgb + 2;
        r1 = rgb + 3;
        g1 = rgb + 4;
        b1 = rgb + 5;
    }
}


