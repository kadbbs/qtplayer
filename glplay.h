#ifndef GLPLAY_H
#define GLPLAY_H
#include <QWidget>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QImage>

#include <cmath>


class glplay:public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit glplay(QWidget *parent= nullptr);
    ~glplay() override;
protected:
    // 初始化OpenGL相关的内容
    void initializeGL() override;
    // 当窗口大小改变时调用
    void resizeGL(int width, int height) override;
    // 渲染函数
    void paintGL() override;
private:
    void timeout();
    void loadTexture();

    QOpenGLTexture *texture;
private:
    QTimer *timer;
    float angle;
    QOpenGLTexture *texture_;


};

#endif // GLPLAY_H
