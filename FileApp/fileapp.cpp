#include "fileapp.h"
#include "General.h"
#include "ui_fileapp.h"

#include <QDir>

FileApp::FileApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileApp), currentDir("/")
{
    ui->setupUi(this);
    setFixedSize(windowSize);
    setStyleSheet(winStyle);
    ui->verticalLayout->setStretchFactor(ui->topLayout, appstatusStretch);
    ui->verticalLayout->setStretchFactor(ui->FileTree, appBottomStretch);

    QPixmap buttonImg(buttonImgPath);
    ui->ReturnButton->setIcon(buttonImg);
    ui->ReturnButton->setIconSize(returnButtonIconSize);
    ui->ReturnButton->setFixedSize(returnButtonSize);
    ui->ReturnButton->setStyleSheet(returnButtonStyle);

    // 读取 / 目录下的所有文件和文件夹
    QIcon folderIcon(folderIconPath);  // 替换为你的文件夹图标路径
    QIcon fileIcon(fileIconPath);      // 替换为你的文件图标路径

    ui->FileTree->setIconSize(QSize(itemSize.height() * 0.8, itemSize.height() * 0.8));
    updateFileTree(currentDir);
}

FileApp::~FileApp()
{
    delete ui;
}

void FileApp::on_ReturnButton_clicked()
{
    this->close();
    emit showMain();
}


void FileApp::on_FileTree_itemDoubleClicked(QListWidgetItem *item)
{
    QString itemText = item->text();

    if (itemText == "..") {
        // 返回上一级目录
        QDir dir(currentDir);
        dir.cdUp();
        updateFileTree(dir.absolutePath());
    } else {
        // 检查是否为目录
        QString newPath = currentDir + "/" + itemText;
        QFileInfo fileInfo(newPath);

        if (fileInfo.isDir()) {
            // 如果是目录，进入该目录
            updateFileTree(newPath);
        } else {
            // 如果是文件，忽略操作
            return;
        }
    }
}

void FileApp::updateFileTree(const QString &path)
{
    QIcon folderIcon(folderIconPath);  // 替换为你的文件夹图标路径
    QIcon fileIcon(fileIconPath);      // 替换为你的文件图标路径

    ui->FileTree->clear();  // 清除当前的所有项

    // 如果不是根目录，添加“..”项以返回上一级
    if (path != "/") {
        QListWidgetItem *parentItem = new QListWidgetItem("..", ui->FileTree);
        parentItem->setIcon(folderIcon);
        parentItem->setSizeHint(itemSize);
        parentItem->setFont(QFont("Arial", 12));
    }

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    // 分别存储文件夹和文件
    QList<QFileInfo> folders;
    QList<QFileInfo> files;

    foreach (QFileInfo fileInfo, list) {
        if (fileInfo.isDir()) {
            folders.append(fileInfo);
        } else if (fileInfo.isFile()) {
            files.append(fileInfo);
        }
    }

    // 对文件夹和文件按名称排序
    std::sort(folders.begin(), folders.end(), [](const QFileInfo &a, const QFileInfo &b) {
        return a.fileName().toLower() < b.fileName().toLower();
    });

    std::sort(files.begin(), files.end(), [](const QFileInfo &a, const QFileInfo &b) {
        return a.fileName().toLower() < b.fileName().toLower();
    });

    // 将文件夹添加到QListWidget
    foreach (QFileInfo folderInfo, folders) {
        QListWidgetItem *item = new QListWidgetItem(folderInfo.fileName(), ui->FileTree);
        item->setIcon(folderIcon);
        item->setSizeHint(itemSize);
        item->setFont(QFont("Arial", 12));
    }

    // 将文件添加到QListWidget
    foreach (QFileInfo fileInfo, files) {
        QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName(), ui->FileTree);
        item->setIcon(fileIcon);
        item->setSizeHint(itemSize);
        item->setFont(QFont("Arial", 12));
    }

    // 更新当前路径
    currentDir = path;
}

