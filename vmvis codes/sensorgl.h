#ifndef SENSORGL_H
#define SENSORGL_H

#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>

class sensorGL : public QGLWidget
{
    Q_OBJECT
    void draw();
    int xRot;
    int yRot;
    int zRot;
public:
    explicit sensorGL(QWidget *parent = 0);
        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);

public slots:
        void setXRotation(int angle);
        void setYRotation(int angle);
        void setZRotation(int angle);

};

#endif // SENSORGL_H
