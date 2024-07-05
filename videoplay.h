#ifndef VIDEOPLAY_H
#define VIDEOPLAY_H

#include <QHBoxLayout>
#include <QObject>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QOpenGLWidget>



class videoplay
{

public:
    videoplay(QWidget *parentWidget,QMediaPlayer* p);
    void vpaly(QWidget *parentWidget,QWidget *vp);


public:
    QString path_="/home/bbs/code/av/outshort.mp4";
    QMediaPlayer *player;

private:
    QMediaPlaylist *playlist;

    QVideoWidget *videoWidget;

};


#endif // VIDEOPLAY_H
