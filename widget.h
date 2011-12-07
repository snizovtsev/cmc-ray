#ifndef WIDGET_H
#define WIDGET_H

#include <QtOpenGL>
#include <QTime>

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
    QGLShaderProgram *  program;
    GLfloat xRot, yRot, zRot;
    GLfloat xVel, yVel, zVel;
    GLfloat zoom;

    QPoint lastPos;
    QTime  lastTime;

    static void accumRotation(GLfloat &rot, GLfloat& vel);
};

#endif // WIDGET_H
