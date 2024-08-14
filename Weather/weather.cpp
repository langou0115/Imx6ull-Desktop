#include "weather.h"
#include "General.h"
#include <QUrl>
#include <QDebug>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QSslConfiguration>
#include <QSslSocket>

Weather::Weather(QObject *parent)
    : QObject{parent}, manager(new QNetworkAccessManager())
{
    connect(manager, &QNetworkAccessManager::finished, this, &Weather::onFinished);
}
void Weather::getWeather()
{
    // 发起 GET 请求
    QUrl url(weatherApi);
    QNetworkRequest request(url);
    manager->get(request);
}

void Weather::onFinished(QNetworkReply *reply)
{
    // 处理响应
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();

        // 解析 JSON 数据
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        if (!jsonDoc.isNull()) {
            QJsonObject jsonObj = jsonDoc.object();
            QJsonArray livesArray = jsonObj["lives"].toArray();
            QJsonObject livesObj = livesArray.at(0).toObject();
            QString weather_ = livesObj["weather"].toString();
            QString temp = livesObj["temperature"].toString();
            QString province = livesObj["province"].toString();
            QString city = livesObj["city"].toString();
            qDebug() << "Weather: " << weather_ << " Temp: " << temp << " Province: " << province << " City: " << city;
            emit sendWeather(weather_, temp, province, city);
        } else {
            emit getWeatherError();
            qDebug() << "Failed to parse JSON.";
        }
    } else {
        emit getWeatherError();
        qDebug() << "Network error: " << reply->errorString();
    }

    reply->deleteLater();
}
