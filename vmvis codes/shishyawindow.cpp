#include "shishyawindow.h"
#include "ui_shishyawindow.h"

shishyawindow::shishyawindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::shishyawindow)
{
    ui->setupUi(this);
    // General specification for the window set the name for window and fixed size.
    this->setWindowTitle("Client Page");
    this->setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->setFixedSize(this->width(),this->height());
    //sensor reseving data form server
    connect(this,SIGNAL(setXvaluex(int)),ui->sensordata,SLOT(setXRotation(int)));
    connect(this,SIGNAL(setYvaluey(int)),ui->sensordata,SLOT(setYRotation(int)));
    connect(this,SIGNAL(setZvaluez(int)),ui->sensordata,SLOT(setZRotation(int)));

    // action
    connect(ui->actionPrint,SIGNAL(triggered()),ui->terminal,SLOT(print()));

}

shishyawindow::~shishyawindow()
{
    delete ui;
}
// Giving the values for the labels
void shishyawindow::setvalue()
{
    ui->ipout->setText(ipvalue);
    ui->portout->setText(portvalue);
}
// chaining the data from the server to the application
void shishyawindow::setX(int value)
{
    emit setXvaluex(value);
}
void shishyawindow::setY(int value)
{
    emit setYvaluey(value);
}
void shishyawindow::setZ(int value)
{
    emit setZvaluez(value);
}
void shishyawindow::getsensor(QByteArray data)
{
    ui->terminal->putData(data);
}

// call the disconnect and close of the client page and setup.
void shishyawindow::on_disconnect_clicked()
{
    emit disconnecttrig();
}


void shishyawindow::on_actionClose_triggered()
{
    this->close();
}

void shishyawindow::on_actionAbout_vmvis_V1_1_triggered()
{
    QMessageBox::information(this, "About VMVIS","The Application is a simple QT GUI design for Communication like TCP/IP,\n Camera and Serial port connections.");
}

void shishyawindow::on_actionHelp_triggered()
{
    QMessageBox::information(this, "Need help","Please Visit www.vishnumuthu.com for support and \n feel free to contact me.\n v.vishnumuthu@gmail.com");
}
