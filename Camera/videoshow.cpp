#include "videoshow.h"
#include <QPainter>

VideoShow::VideoShow(QWidget *parent)
    : QWidget{parent}
{}

void VideoShow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(!img.isNull()){
        painter.drawImage(QPointF(0,0),img);
    }
}
