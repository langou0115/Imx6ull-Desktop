#ifndef VIDEOSHOW_H
#define VIDEOSHOW_H

#include <QWidget>

class VideoShow : public QWidget
{
    Q_OBJECT
public:
    explicit VideoShow(QWidget *parent = nullptr);
    /* 显示的每帧图像 */
    QImage img;
signals:

protected:
    void paintEvent(QPaintEvent *) override;
};

#endif // VIDEOSHOW_H
