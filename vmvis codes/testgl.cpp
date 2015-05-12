#include "testgl.h"
#include "ui_testgl.h"

testGL::testGL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::testGL)
{
    ui->setupUi(this);
    // General specification for the guruwindow # Hide the full screen button and set the name for window and fixed size.
    this->setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle("Sensor Test");
    this->setFixedSize(this->width(),this->height());
}

testGL::~testGL()
{
    delete ui;
}
// sends the changed value to the gl design
void testGL::on_slidexdir_valueChanged(int value)
{
    emit sendx(value);
}

void testGL::on_slideydir_valueChanged(int value)
{
    emit sendy(value);
}

void testGL::on_slidezdir_valueChanged(int value)
{
    emit sendz(value);
}
