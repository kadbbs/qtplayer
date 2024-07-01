#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QListView>
#include <QMouseEvent>

class MyListView : public QListView
{
    Q_OBJECT

public:
    explicit MyListView(QWidget *parent = nullptr);

signals:
    void rightClicked(const QModelIndex &index, const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // MYLISTVIEW_H
