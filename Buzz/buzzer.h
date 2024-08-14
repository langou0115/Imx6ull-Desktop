#ifndef BUZZER_H
#define BUZZER_H

#include <QFile>
#include <QObject>

class Buzzer : public QObject
{
    Q_OBJECT
public:
    explicit Buzzer(QObject *parent = nullptr);

    void setBuzzerOn();
    void setBuzzerOff();
signals:

private:
    QFile *buzzerFile;
};

#endif // BUZZER_H
