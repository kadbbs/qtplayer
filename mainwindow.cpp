#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(new QMediaPlayer(this) ),    isFullscreen(false)

{
    ui->setupUi(this);
    positionUpdateTimer=(new QTimer(this));

    model = new QStringListModel(this);
    ui->crrent_file->setModel(model);

    vp=new videoplay(this,player);
    ui->positionSlider->setTickPosition(QSlider::TicksBelow);
    ui->positionSlider->setTickInterval(1000);

    connect(ui->openfile,&QAction::triggered,this,&MainWindow::openfile);
    connect(ui->help,&QAction::triggered,this,&MainWindow::showhelp);
    connect(ui->crrent_file, &MyListView::clicked, this, &MainWindow::onListViewClicked);
    connect(ui->actionpause, &QAction::triggered, this, &MainWindow::vpause);
    connect(ui->actionstart, &QAction::triggered, this, &MainWindow::vstart);
    connect(ui->button_start,&QPushButton::clicked,this,&MainWindow::vstart);
    connect(ui->button_pause, &QPushButton::clicked, this, &MainWindow::vpause);
    connect(ui->stop, &QPushButton::clicked, this, &MainWindow::vstop);
    connect(ui->crrent_file, &MyListView::rightClicked, this, &MainWindow::handleRightClick);


#if 0
    PlayerControls *controls = new PlayerControls(this);
    controls->setState(player->state());
    controls->setVolume(player->volume());
    controls->setMuted(controls->isMuted());

    connect(controls, &PlayerControls::play, player, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, player, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, player, &QMediaPlayer::stop);

    connect(controls, &PlayerControls::changeVolume, player, &QMediaPlayer::setVolume);
    connect(controls, &PlayerControls::changeMuting, player, &QMediaPlayer::setMuted);
    connect(controls, &PlayerControls::changeRate, player, &QMediaPlayer::setPlaybackRate);

    connect(player, &QMediaPlayer::stateChanged, controls, &PlayerControls::setState);
    connect(player, &QMediaPlayer::volumeChanged, controls, &PlayerControls::setVolume);
    connect(player, &QMediaPlayer::mutedChanged, controls, &PlayerControls::setMuted);

#endif
    // 连接滑块信号和槽函数
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    qDebug()<<"&QMediaPlayer::positionChanged";
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(ui->positionSlider, &QSlider::sliderMoved, this, &MainWindow::setPosition);

    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::setSpeed);
    ui->comboBox->addItem("1.0x", 1.0);
    ui->comboBox->addItem("1.5x", 1.5);
    ui->comboBox->addItem("2.0x", 2.0);

    ui->positionSlider->setRange(0,player->duration()/1000);
    m_labelDuration = new QLabel(this);
    connect(ui->positionSlider, &QSlider::sliderMoved, this, &MainWindow::seek);
    ui->widget->setLayout(new QVBoxLayout);




    imageLabel = new QLabel(ui->widget);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 加载并设置图片
    QString imagePath="/home/bbs/Pictures/bbplayer.png";

    QPixmap pixmap(imagePath);
    imageLabel->setPixmap(pixmap.scaled(ui->widget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->adjustSize();  // 调整 QLabel 大小以适应图片

    //volumeSlider
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(player->volume());

    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::setVolume);

    readline();
    this->installEventFilter(this);

}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (!imageLabel->pixmap()->isNull()) {
        imageLabel->setPixmap(imageLabel->pixmap()->scaled(ui->widget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{

    // 设置stackedWidget
//    stackedWidget->addWidget(this);
//    stackedWidget->addWidget(vp->videoWidget);
//    stackedWidget->setCurrentIndex(1); // 显示第二个小部件

//    setCentralWidget(stackedWidget);
            if (vp->videoWidget->isFullScreen()) {
                vp->videoWidget->setFullScreen(false);

            } else {
                vp->videoWidget->setFullScreen(true);
            }
//    if (event->button() == Qt::LeftButton) {
//        toggleFullscreen();
//    }
//    QMainWindow::mouseDoubleClickEvent(event);
//    if (this->isFullScreen()) {
//        this->showNormal();

//    } else {
//        this->showFullScreen();
//    }
//toggleFullscreen();

//    if (event->button() == Qt::LeftButton && ui->widget->rect().contains(event->pos())) {
//        toggleFullscreen();
//    }
//    QMainWindow::mouseDoubleClickEvent(event);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && vp->videoWidget->isFullScreen()) {
        vp->videoWidget->setFullScreen(false); // 退出全屏模式
        qDebug()<<"Key_Escape退出全屏模式";
        this->showNormal();  // 恢复到正常窗口模式
        isFullscreen = false;    }
    QMainWindow::keyPressEvent(event);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape && isFullscreen) {
            toggleFullscreen();
            qDebug()<<"Key_Escape退出全屏模式";

            return true;
        }
    }
    // 调用基类方法以确保其他事件被处理
    return QMainWindow::eventFilter(obj, event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_crrent_file()
{
    QStringList stringList;
    for (const QString &line : pathlist) {
        stringList.append(line);
    }

    model->setStringList(stringList);
}

void MainWindow::readline()
{
    QFile file("/home/bbs/code/Qt/learn1/tmpf");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        pathlist.push_back(line);
    }

    file.close();
    update_crrent_file();

}

void MainWindow::openfile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*)"));
    if (!fileName.isEmpty()) {

        path=fileName;
        pathlist.push_back(path);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Cannot open file:\n%1").arg(fileName));
        }

    qDebug()<<path;
    qDebug()<<pathlist;

    QFile file_("/home/bbs/code/Qt/learn1/tmpf");

    // 尝试以写入模式打开文件，这会清空文件内容
    if (file_.open(QIODevice::WriteOnly)) {
        file_.close(); // 关闭文件，此时文件内容已被清空
    } else {
        qDebug() << "无法打开文件以进行清空：" << file_.errorString();
    }

    QFile file("/home/bbs/code/Qt/learn1/tmpf");
    if(!file.exists()){
        qDebug()<<"file is not eixt";
    }
    int res=file.open(QIODevice::ReadWrite | QIODevice::Text|QIODevice::Append);
    if(!res){
        qDebug() << "无法打开文件：" << file.errorString();
        return ;
    }

    for (const QString &item : pathlist) {
        file.write(item.toUtf8()+"\n");

    }
    file.close();



    update_crrent_file();

}

void MainWindow::showhelp()
{
    QDialog *help =new QDialog(this);
    qDebug()<<"app help is xxx ";

}

void MainWindow::onListViewClicked(const QModelIndex &index)
{
    // 处理点击事件的槽函数
    qDebug() << "Clicked item: " << index.row();
    qDebug() << "Text: " << index.data().toString();

    if (imageLabel) {
        delete imageLabel;
        imageLabel = nullptr;
    }

//    if(vp!=nullptr){
//        delete vp;
//    }

    vp->path_=index.data().toString();
    vp->vpaly(this,ui->widget);



#if 0
    qDebug()<<"vp->player->duration() is : "<<vp->player->duration();
    ui->positionSlider->setMaximum(vp->player->duration());
#endif



}

void MainWindow::vpause()
{
    vp->player->pause();
}

void MainWindow::vstart()
{
    vp->player->play();
}

void MainWindow::vstop()
{
    vp->player->stop();
}

void MainWindow::updatePosition(qint64 position)
{
//    ui->positionSlider->setValue(position);
//    currentPosition = position;
//    if (!ui->positionSlider->isSliderDown())
    ui->positionSlider->setValue(position / 1000);
    QTime time = QTime(0, 0).addSecs(position / 1000);
    ui->timeshow->setText(time.toString("HH:mm:ss")+"/"+totaltime);
}

void MainWindow::setPosition(int position)
{
    vp->player->setPosition(position);

}

void MainWindow::updateDuration(qint64 duration)
{

    ui->positionSlider->setMaximum(duration / 1000);
    QTime time = QTime(0, 0).addSecs(duration / 1000);
    totaltime=time.toString("HH:mm:ss");
//    ui->alltime->setText("/"+totaltime);


}

void MainWindow::updateSliderPosition()
{
    ui->positionSlider->setValue(currentPosition);
}

void MainWindow::handleRightClick(const QModelIndex &index, const QPoint &pos)
{
    qDebug()<<"handleRightClick";
    if (!index.isValid()) {
        qDebug()<<"RightClick";


        QMenu contextMenu(this);
        QAction action1("添加", this);
        connect(&action1, &QAction::triggered, this, [this]() {

//            QLineEdit *lineEdit = new QLineEdit(this);
//            lineEdit->setPlaceholderText("Enter path here");


//            // 处理用户输入
//            connect(lineEdit, &QLineEdit::editingFinished, this, [this,&lineEdit]() {
//                pathlist.push_back(lineEdit->text());
//                update_crrent_file();
//            });
//            update_crrent_file();
            bool ok;
            QString text = QInputDialog::getText(this, tr("添加路径"),
                                                 tr("路径:"), QLineEdit::Normal,
                                                 "", &ok);
            if (ok && !text.isEmpty()) {
                pathlist.push_back(text);
                update_crrent_file();
            }

        });

        contextMenu.addAction(&action1);

        contextMenu.exec(ui->crrent_file->mapToGlobal(pos));
        return;

    }

    QMenu contextMenu(this);
    QAction action1("删除", this);
    QAction action2("添加", this);
    connect(&action1, &QAction::triggered, this, [this,index]() {
        qDebug() << "删除 triggered on item at row" << index.row();
        pathlist.removeAll(index.data().toString());
        update_crrent_file();
    });
    connect(&action2, &QAction::triggered, this, [this,index]() {
        qDebug() << "添加 triggered on item at row" << index.row();
        QLineEdit *lineEdit = new QLineEdit(this);
        pathlist.push_back(lineEdit->text());
        update_crrent_file();

    });

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);

    contextMenu.exec(ui->crrent_file->mapToGlobal(pos));
}

void MainWindow::seek(int seconds)
{
    player->setPosition(seconds * 1000);

}

void MainWindow::setSpeed(const QString &text)
{
    qDebug() << "Setting text is:" << text;
    QString tmp=text;
    tmp.chop(1);


    qreal speed = tmp.toDouble();
    qDebug() << "Setting playback rate to:" << speed;


qint64 currentPosition = player->position();

    player->setPlaybackRate(speed);
    player->pause();

    player->setPosition(currentPosition);


    player->play();



}

void MainWindow::setVolume(int value)
{
    player->setVolume(value);

}

void MainWindow::toggleFullscreen()
{
//    if (isFullscreen) {
//        vp->videoWidget->setFullScreen(false);
//        this->showNormal();  // 恢复到正常模式
//        qDebug()<<"        this->showNormal()恢复到正常模式";
//        isFullscreen = false;
//    } else {
//        this->showFullScreen();  // 进入全屏模式
//        vp->videoWidget->setFullScreen(true);
//        isFullscreen = true;
//    }
    if (isFullscreen) {
        stackedWidget->setCurrentWidget(this);  // 显示主要窗口
        isFullscreen = false;
    } else {
        stackedWidget->setCurrentWidget(vp->videoWidget);  // 显示全屏视频窗口
        isFullscreen = true;
    }
}
