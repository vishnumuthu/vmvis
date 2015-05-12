#include "gurutcp.h"

gurutcp::gurutcp(QObject *parent) :
    QObject(parent)
{   
}
// <1.1> Start the server process
// when the connect button is clicked the sever object is created and server is started.
void gurutcp::start_connection()
{
    server = new QTcpServer(this);
    // wait for new client connection
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    // listen from incoming port and ip
    if(!server->listen( ipaddress , setpost ))
    {
        QMessageBox::warning(0, "Error","Error starting the server, try again later");
    }
    else
    {
        QMessageBox::information(0, "Welcome","Server started successfully.");
    }

}
// when there is a new connection, a socket is created and the connection is established.
void gurutcp::newConnection()
{
    // new client socket is created when there is a new connection.
    socket = server->nextPendingConnection();
    // gives the connection for the client only when there is a connection.
    emit senddata();
    // wait to read the data if something appears from the connected client.
    connect(socket, SIGNAL(readyRead()),this, SLOT(readdata()));
}
// thee server is closed when the disconnect button is clicked.
void gurutcp::disconnectserver()
{
    server->close();
}
// </1.1>
// <1.2> wait for the new data
void gurutcp::readdata()
{
    QByteArray first = socket->readAll();
    // the received password data is checked here.
    if(first == "Hello"+verifypass)
    {
        socket->write("ok");
        socket->flush();
    }
    else
    {
        socket->write("notok");
        socket->flush();
    }
}
// </1.2>
// <1.3> these function are activated only when there is a new client connection
void gurutcp::setXchange(int value)
{
    QByteArray valuex = QByteArray::number(value);
    socket->write("x"+valuex);
}
void gurutcp::setYchange(int value)
{
    QByteArray valuey = QByteArray::number(value);
    socket->write("y"+valuey);
}
void gurutcp::setZchange(int value)
{
    QByteArray valuez = QByteArray::number(value);
    socket->write("z"+valuez);
}
// </1.3>
