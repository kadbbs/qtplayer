#include "videoplay.h"
videoplay::videoplay(QWidget *parentWidget,QMediaPlayer* p):player(p)
{
//    player = new QMediaPlayer(parentWidget);
    videoWidget = new QVideoWidget(parentWidget);
}



void videoplay::vpaly(QWidget *parentWidget,QWidget *vp)
{


//    QLayout *layout = vp->parentWidget()->layout();
//    int index = layout->indexOf(parentWidget);
//    layout->removeWidget(vp);

//    if(player!=nullptr){
//        delete player;
//        vp->layout()->removeWidget(videoWidget);

//    }


//    layout->addWidget(videoWidget);

    vp->layout()->addWidget(videoWidget);



    player->setMedia(QUrl::fromLocalFile(path_));
    //playlist->addMedia(QUrl("http://example.com/myclip2.mp4"));


    player->setVideoOutput(videoWidget);

    player->play();


}
