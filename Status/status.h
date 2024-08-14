#ifndef STATUS_H
#define STATUS_H

#include <QFile>
#include <QObject>
#include <QThread>
#include <QTimer>

class Status : public QThread
{
    Q_OBJECT
public:
    explicit Status(QObject *parent = nullptr);

signals:
    void updateStatus(QString, QString, QString);
private:
    QTimer *timer;
    QFile *cputemp;
    void updateDateTime();
protected:
    void run() override;
};

#endif // STATUS_H
