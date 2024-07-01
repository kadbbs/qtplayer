#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    positionUpdateTimer=(new QTimer(this));

    model = new QStringListModel(this);
    ui->crrent_file->setModel(model);

    vp=new videoplay(this);
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

    connect(positionUpdateTimer, &QTimer::timeout, this, &MainWindow::updateSliderPosition);
    positionUpdateTimer->start(500); // Update every 100 milliseconds


    readline();
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

//    if(vp!=nullptr){
//        delete vp;
//    }

    vp->path_=index.data().toString();
    vp->vpaly(this,ui->widget);
    // 连接滑块信号和槽函数
    connect(vp->player, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    qDebug()<<"&QMediaPlayer::positionChanged";
    connect(vp->player, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(ui->positionSlider, &QSlider::valueChanged, this, &MainWindow::setPosition);
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
    currentPosition = position;
}

void MainWindow::setPosition(int position)
{
    vp->player->setPosition(position);

}

void MainWindow::updateDuration(qint64 duration)
{
    ui->positionSlider->setMaximum(duration);


}

void MainWindow::updateSliderPosition()
{
    ui->positionSlider->setValue(currentPosition);
}

void MainWindow::handleRightClick(const QModelIndex &index, const QPoint &pos)
{
    qDebug()<<"handleRightClick";
    if (!index.isValid()) {
        return;
    }

    QMenu contextMenu(this);
    QAction action1("Action 1", this);
    QAction action2("Action 2", this);
    connect(&action1, &QAction::triggered, this, [index]() {
        qDebug() << "Action 1 triggered on item at row" << index.row();
    });
    connect(&action2, &QAction::triggered, this, [index]() {
        qDebug() << "Action 2 triggered on item at row" << index.row();
    });

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);

    contextMenu.exec(ui->crrent_file->mapToGlobal(pos));
}
