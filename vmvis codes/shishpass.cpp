#include "shishpass.h"
#include "ui_shishpass.h"

shishpass::shishpass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shishpass)
{
    ui->setupUi(this);
    // General specification for the window set the name for window and fixed size.
    this->setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle("Client Setup");
    this->setFixedSize(this->width(),this->height());

    //sensor reseving data form server
    connect(this,SIGNAL(sendX(int)),&start_shishya,SLOT(setX(int)));
    connect(this,SIGNAL(sendY(int)),&start_shishya,SLOT(setY(int)));
    connect(this,SIGNAL(sendZ(int)),&start_shishya,SLOT(setZ(int)));
    connect(this,SIGNAL(sensordata(QByteArray)),&start_shishya,SLOT(getsensor(QByteArray)));
    // Passing value
    connect(ui->clientconnect,SIGNAL(clicked()),&start_shishya,SLOT(setvalue()));
    // close connection
    connect(&start_shishya,SIGNAL(disconnecttrig()),this,SLOT(disconnectclient()));
}

shishpass::~shishpass()
{
    delete ui;
}
// <1.1> edit chick for IP, Port and Password
// check the valid entered ip
void shishpass::on_ipreq_editingFinished()
{
    bool ipcheck = ipreq.setAddress(ui->ipreq->text());
    if(!ipcheck)
    {
        QMessageBox::information(this, "IP Value","Enter correct IP Vaule");
        ui->iplabel->setStyleSheet("QLabel { color : red; }");
    }
    else
        ui->iplabel->setStyleSheet("QLabel { color : black; }");
}
// check the valid check for port
void shishpass::on_portreq_editingFinished()
{
    QString portvalue = ui->portreq->text();
    bool check;
    portvalue.toUShort(&check);
    if(!check)
    {
        QMessageBox::information(this, "Port Value","Enter correct Port Vaule");
        ui->portlabel->setStyleSheet("QLabel { color : red; }");
    }
    else
        ui->portlabel->setStyleSheet("QLabel { color : black; }");
}
// check the valid check for password
void shishpass::on_passreq_editingFinished()
{
    if(ui->passreq->text()=='\0')
    {
        QMessageBox::information(this, "Password Missing","Enter Password");
        ui->passlabel->setStyleSheet("QLabel { color : red; }");
    }
    else
    {
        ui->passlabel->setStyleSheet("QLabel { color : black; }");
    }
}
// </1.1>
// <1.2> Pushbutton to check the input and to initiate the client.
void shishpass::on_clientconnect_clicked()
{
    // checking for IP address
    bool ipcheck = ipreq.setAddress(ui->ipreq->text());

    // checking for Port value
    QString portvalue = ui->portreq->text();
    bool check;
    quint16 port = portvalue.toUShort(&check);
    // Reading the password
    QByteArray passreq = ui->passreq->text().toLatin1();
    // check for all
    if(ipcheck & check & (ui->passreq->text()!='\0'))
    {
        // read the value for and display in the label inside the client page
        start_shishya.ipvalue = ui->ipreq->text();
        start_shishya.portvalue = ui->portreq->text();
        // new socket is created
        socket = new QTcpSocket(this);
        // Requesting the connection to the server.
        socket->connectToHost(ipreq,port);
        // If the connection is valid
        if(socket->waitForConnected(5000))
        {
          // send welcome message and password
          socket->write("Hello");
          socket->write(passreq);
          socket->waitForBytesWritten(1000);
          socket->waitForReadyRead(3000);

          // Read the response from the server
          QByteArray second = socket->readAll();
          // If the connection is ok open the client page and wait for the new data to read.
          if(second == "ok")
          {
              this->close();
              start_shishya.show();
              connect(socket, SIGNAL(readyRead()),this, SLOT(readresponse()));
          }
           else
          {
           // close the connection
           socket->close();
          }
          // if the connection is not accepted.
          if(second == "notok")
          {
           QMessageBox::warning(0, "Error","Enter correct password to connect");
          }
        }
        else
        {
           QMessageBox::warning(0, "Error","Enter different IP or Port and Try again");
        }
    }
    else
    {
    if(!ipcheck)
    {
        QMessageBox::information(this, "IP Value","Enter correct IP Vaule");
        ui->iplabel->setStyleSheet("QLabel { color : red; }");
    }
    else
        ui->iplabel->setStyleSheet("QLabel { color : black; }");


    if(!check)
    {
        QMessageBox::information(this, "Port Value","Enter correct Port Vaule");
        ui->portlabel->setStyleSheet("QLabel { color : red; }");
    }
    else
        ui->portlabel->setStyleSheet("QLabel { color : black; }");

    if(ui->passreq->text()=='\0')
    {
        QMessageBox::information(this, "Password Missing","Enter Password");
        ui->passlabel->setStyleSheet("QLabel { color : red; }");
    }
    else
    {
        ui->passlabel->setStyleSheet("QLabel { color : black; }");
    }
    }
}
// </1.2>
// <1.3> Wait and read the sensor data.
void shishpass::readresponse()
{
    // read the incoming sensor data
    QByteArray second = socket->readAll();
    emit sensordata(second);
    // Filter the data based on the axis
    if(second.size() > 5)
    {
        QByteArray xval;
        QByteArray yval;
        if(second.at(0) == 'x')
        {
            second.remove(0,1);
            for(int i = 0; i<second.size();i++)
            {
                if(second.at(i) != 'y')
                    xval[i] = second[i];
                else
                break;
            }
            second.remove(0,xval.size());
            int value = xval.toInt(false, 10);
            emit sendX(value);
        }
        if(second.at(0) == 'y')
        {
            second.remove(0,1);
            for(int i = 0; i<second.size();i++)
            {
                if(second.at(i) != 'z')
                    yval[i] = second[i];
                else
                    break;
            }
            second.remove(0,yval.size());
            int value = yval.toInt(false, 10);
            emit sendY(value);
        }
        if(second.at(0) == 'z')
        {
            second.remove(0,1);
            int value = second.toInt(false, 10);
            emit sendZ(value);
        }
    }
    else
    {
    if(second.at(0) == 'x')
    {
        second.remove(0,1);
        int value = second.toInt(false, 10);
        emit sendX(value);
    }
    else if(second.at(0) == 'y')
    {
        second.remove(0,1);
        int value = second.toInt(false, 10);
        emit sendY(value);
    }
    else if(second.at(0) == 'z')
    {
        second.remove(0,1);
        int value = second.toInt(false, 10);
        emit sendZ(value);
    }
    }
}
// </1.3>
// <1.4> Disconnect the client when disconnect button is clicked, close the page and open the setup page again.
void shishpass::disconnectclient()
{
    socket->close();
    start_shishya.close();
    this->show();
}
// </1.4>
