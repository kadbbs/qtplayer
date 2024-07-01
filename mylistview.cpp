#include "mylistview.h"



MyListView::MyListView(QWidget *parent): QListView(parent)
{

}

void MyListView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QModelIndex index = indexAt(event->pos());
        emit rightClicked(index, event->pos());
    }
    QListView::mousePressEvent(event);

}
