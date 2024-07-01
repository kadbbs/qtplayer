#include "videoplay.h"
videoplay::videoplay(QWidget *parentWidget)
{
//    player = new QMediaPlayer(parentWidget);
}



void videoplay::vpaly(QWidget *parentWidget,QWidget *vp)
{


//    QLayout *layout = vp->parentWidget()->layout();
//    int index = layout->indexOf(parentWidget);
//    layout->removeWidget(vp);

    if(player!=nullptr){
        delete player;
        vp->layout()->removeWidget(videoWidget);

    }
    player = new QMediaPlayer(parentWidget);


    player->setMedia(QUrl::fromLocalFile(path_));
    //playlist->addMedia(QUrl("http://example.com/myclip2.mp4"));

    videoWidget = new QVideoWidget(parentWidget);
//    layout->addWidget(videoWidget);
    vp->setLayout(new QVBoxLayout);
    vp->layout()->addWidget(videoWidget);
    player->setVideoOutput(videoWidget);

    player->play();

}
