#ifndef WIDGET_H
#define WIDGET_H

#include <QtCore>
#include <QtOpenGL>

#include "scene.h"
#include "xmlreader.h"


/**
  @class Widget
  Класс для создания и настройки окна вывода.
  Хранит контекст OpenGL, подключает шейдеры и обрабатывает
  пользовтельский ввод (вращение сцены).
  */
class Widget : public QGLWidget
{
    Q_OBJECT
public:
    Widget(QGLWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    /// Хранит шейдерные программы.
    QGLShaderProgram *program;
    XMLReader         reader;
    /// Хранит сцену.
    Scene             scene;

    /// Поворот сцены относительной каждой оси.
    GLfloat xRot, yRot, zRot;
    /// Скорости затухающего движения поворота.
    GLfloat xVel, yVel, zVel;
    /// Масштаб.
    GLfloat zoom;

    QPoint lastPos;
    QTime  lastTime;

    static void accumRotation(GLfloat &rot, GLfloat& vel);
};

#endif // WIDGET_H
