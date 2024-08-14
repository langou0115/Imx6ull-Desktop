#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QListWidget>

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);
    ~Player();

signals:
    void showMain();

private slots:
    void on_ReturnButton_clicked();

    void on_start_clicked();

private:
    Ui::Player *ui;
    QVideoWidget *videoWidget;
    QMediaPlayer *player;
    QListWidget *listWidget;
    bool isPlaying = false;
    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
};



#endif // PLAYER_H
