#include "widget.h"

Widget::Widget(QGLWidget *parent)
    : QGLWidget(QGLFormat(QGL::NoDepthBuffer | QGL::NoStencilBuffer | QGL::SampleBuffers), parent),
      program(0),
      reader("scene.xml"), scene(&reader),
      xRot(0), yRot(0), zRot(0),
      xVel(0), yVel(0), zVel(0),
      zoom(1)
{
    startTimer(20);
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    program = new QGLShaderProgram(context(), this);
    qDebug() << "Compiling vertex shader...";
    program->addShaderFromSourceFile(QGLShader::Vertex, ":/main.vert");
    qDebug() << "Compiling fragment shaders...";
    scene.makeShaders(program);
    qDebug() << "Linking...";
    program->link();
    program->bind();

    qDebug() << "Loading textures...";
    QImage texture(":/floor.jpg");
    texture = convertToGLFormat(texture);

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0, /* level */
                 4, /* internalFormat */
                 texture.width(),
                 texture.height(),
                 0, /* border */
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texture.bits());
    program->setUniformValue("floorTex", 0);
}

void Widget::paintGL()
{
    static QTime perf;
    perf.start();

    if (!program->isLinked())
        this->close();
    glClear(GL_COLOR_BUFFER_BIT);

    QMatrix4x4 m;
    m.rotate(xRot, 1.0, 0.0, 0.0);
    m.rotate(yRot, 0.0, 1.0, 0.0);
    m.rotate(zRot, 0.0, 0.0, 1.0);
    m.scale(zoom);
    program->setUniformValue("matrix", m);

    glBegin(GL_QUADS);
      glVertex2f(-1.0, -1.0);
      glVertex2f(-1.0, 1.0);
      glVertex2f(1.0, 1.0);
      glVertex2f(1.0, -1.0);
    glEnd();
    // Do all drawing before calculating fps
    glFinish();

    program->release();
    renderText(10, 20, QString("FPS: %1").arg(1000.0 / perf.elapsed()),
               QFont("sans", 12, QFont::Bold));
    program->bind();
}

void Widget::accumRotation(GLfloat &rot, GLfloat& vel)
{
    float sign = copysignf(1.0, vel);

    rot += vel;
    vel = copysignf(qMax(fabsf(vel) - 0.5f, 0.0f), sign);
}

void Widget::timerEvent(QTimerEvent *)
{
    accumRotation(xRot, xVel);
    accumRotation(yRot, yVel);
    accumRotation(zRot, zVel);

    updateGL();
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    program->setUniformValue("aspectRatio", (w+.0f) / h);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    lastTime.start();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (lastTime.elapsed() <= 3)
        return;

    double dx = (event->x() - lastPos.x() + .0) / lastTime.elapsed() * 5;
    double dy = (event->y() - lastPos.y() + .0) / lastTime.elapsed() * 5;

    /* Set rotation velocity */
    if (event->buttons() & Qt::LeftButton) {
        xVel = dy;
        yVel = dx;
        zVel = 0.0;
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        xVel = dy;
        yVel = 0.0;
        zVel = dx;
        updateGL();
    }

    lastPos = event->pos();
    lastTime.start();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    zoom *= exp(event->delta() / 1000.0);
    updateGL();
}
