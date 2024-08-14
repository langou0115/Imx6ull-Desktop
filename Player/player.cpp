#include "player.h"
#include "General.h"
#include "ui_player.h"

Player::Player(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Player)
{
    ui->setupUi(this);
    setFixedSize(windowSize);
    setStyleSheet(winStyle);
    ui->bottomlayout->setStretchFactor(ui->toplayout, appstatusStretch);
    ui->bottomlayout->setStretchFactor(ui->bottomlayout, appBottomStretch);

    QPixmap buttonImg(buttonImgPath);
    ui->ReturnButton->setIcon(buttonImg);
    ui->ReturnButton->setIconSize(returnButtonIconSize);
    ui->ReturnButton->setFixedSize(returnButtonSize);
    ui->ReturnButton->setStyleSheet(returnButtonStyle);

    ui->start->setFixedSize(playerButtonSize);
    ui->start->setStyleSheet(playerButtonOnStyle);
    // videoWidget = new QVideoWidget(ui->video);
    videoWidget = new QVideoWidget();
    player = new QMediaPlayer();
    player->setVideoOutput(videoWidget);
    ui->playerlayout->addWidget(videoWidget);
    videoWidget->setStyleSheet("background-color: black;");
    connect(player, &QMediaPlayer::positionChanged, this, &Player::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &Player::updateDuration);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &Player::handleMediaStatusChanged);

    // 相对路径读取文件
    player->setMedia(QUrl("qrc:/Video/oceans.mp4"));

}

Player::~Player()
{
    delete ui;
}

void Player::on_ReturnButton_clicked()
{
    player->stop();
    this->close();
    emit showMain();
}


void Player::on_start_clicked()
{
    if(isPlaying)
    {
        ui->start->setStyleSheet(playerButtonOnStyle);
        player->pause();
        isPlaying = false;
    }
    else
    {
        ui->start->setStyleSheet(playerButtonOffStyle);
        player->play();
        isPlaying = true;
    }
}

void Player::updateDuration(qint64 duration)
{
    ui->progressBar->setMaximum(duration);
}

void Player::updatePosition(qint64 position)
{
    ui->progressBar->setValue(position);
}

void Player::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        videoWidget->setStyleSheet("background-color: black;");
    }
}
