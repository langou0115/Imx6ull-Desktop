#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QMainWindow>

#include <sys/utsname.h>

namespace Ui {
class SystemInfo;
}

class SystemInfo : public QMainWindow
{
    Q_OBJECT

public:
    explicit SystemInfo(QWidget *parent = nullptr);
    ~SystemInfo();

signals:
    void showMain();

private slots:
    void on_ReturnButton_clicked();

private:
    Ui::SystemInfo *ui;
};

#endif // SYSTEMINFO_H
