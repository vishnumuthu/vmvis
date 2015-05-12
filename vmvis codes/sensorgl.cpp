#include "sensorgl.h"

sensorGL::sensorGL(QWidget *parent) :
    QGLWidget(parent)
{
    // initiating the values to zero for the data.
    xRot = 0;
    yRot = 0;
    zRot = 0;
}
// getting the data for the x axis and updating
void sensorGL::setXRotation(int angle)
{
    // if change occurs then update the angle value and the GL design
    if (angle != xRot)
    {
        xRot = angle;
        updateGL();
    }
}

void sensorGL::setYRotation(int angle)
{
    // if change occurs then update the angle value and the GL design
    if (angle != yRot)
    {
        yRot = angle;
        updateGL();
    }
}

void sensorGL::setZRotation(int angle)
{
    // if change occurs then update the angle value and the GL design
    if (angle != zRot)
    {
        zRot = angle;
        updateGL();
    }
}
// calls initializeGL, paintGL and resizeGL when widget is created.
void sensorGL::initializeGL()
{
    qglClearColor(Qt::black);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
}
void sensorGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot , 1.0, 0.0, 0.0);
    glRotatef(yRot , 0.0, 1.0, 0.0);
    glRotatef(zRot , 0.0, 0.0, 1.0);
    draw();
}

void sensorGL::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}
// Creating the required design here.
void sensorGL::draw()
{
    glBegin(GL_QUADS);
    qglColor(Qt::blue);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);
    glEnd();
    glBegin(GL_QUADS);
    qglColor(Qt::green);
        glNormal3f(0,0,1);
        glVertex3f(-0.5,-0.5,1);
        glVertex3f(-0.5,0.5,1);
        glVertex3f(0.5,0.5,1);
        glVertex3f(0.5,-0.5,1);
        glEnd();
    glBegin(GL_QUADS);
    qglColor(Qt::gray);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(-0.5,-0.5,1);
        glVertex3f(0.5,-0.5,1);
    glEnd();
    glBegin(GL_QUADS);
    qglColor(Qt::gray);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0.5,-0.5,1);
        glVertex3f(0.5,0.5,1);
    glEnd();
    glBegin(GL_QUADS);
    qglColor(Qt::gray);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0.5,0.5,1);
        glVertex3f(-0.5,0.5,1);
    glEnd();
    glBegin(GL_QUADS);
    qglColor(Qt::gray);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(-0.5,0.5,1);
        glVertex3f(-0.5,-0.5,1);
    glEnd();
    glBegin(GL_QUADS);
    qglColor(Qt::red);
        glNormal3f(0,0,1);
        glVertex3f(-0.5,-0.5,1.1);
        glVertex3f(-0.25,0,1.1);
        glVertex3f(0.25,0,1.1);
        glVertex3f(0.5,-0.5,1.1);
        glEnd();
    glBegin(GL_TRIANGLES);
    qglColor(Qt::black);
    glNormal3f(0,0,1);
    glVertex3f(-0.25,0,1.1);
    glVertex3f(0,0.5,1.1);
    glVertex3f(0.25,0,1.1);
    glEnd();

}



