#ifndef FILEAPP_H
#define FILEAPP_H

#include <QListWidget>
#include <QWidget>

namespace Ui {
class FileApp;
}

class FileApp : public QWidget
{
    Q_OBJECT

public:
    explicit FileApp(QWidget *parent = nullptr);
    ~FileApp();

private slots:
    void on_ReturnButton_clicked();

    void on_FileTree_itemDoubleClicked(QListWidgetItem *item);

signals:
    void showMain();

private:
    Ui::FileApp *ui;
    QString currentDir;

    void updateFileTree(const QString &path);
};

#endif // FILEAPP_H
