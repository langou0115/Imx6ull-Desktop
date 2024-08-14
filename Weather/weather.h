#ifndef WEATHER_H
#define WEATHER_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QThread>

class Weather : public QObject
{
    Q_OBJECT
public:
    explicit Weather(QObject *parent = nullptr);
    void getWeather();

signals:
    void getWeatherError();
    void sendWeather(QString weather_, QString temp, QString province, QString city);

private slots:
    void onFinished(QNetworkReply *reply);
private:
    QNetworkAccessManager *manager;
};

#endif // WEATHER_H
