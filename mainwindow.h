#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <QMainWindow>
#include <QVector>
#include <QWidget>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QImage>
#include <QStringListModel>
#include <cmath>
#include <QDebug>
#include "videoplay.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void update_crrent_file();
    void readline();

private slots:
    void openfile();
    void showhelp();
    void onListViewClicked(const QModelIndex &index);
    void vpause();
    void vstart();
    void updatePosition(qint64 position);
    void setPosition(int position);
    void updateDuration(qint64 duration);



private:
    Ui::MainWindow *ui;
    QString path;
    QVector<QString> pathlist;
    QStringListModel *model;
    videoplay *vp;


};

#endif // MAINWINDOW_H
