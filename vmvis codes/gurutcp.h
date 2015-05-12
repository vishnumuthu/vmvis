#ifndef GURUTCP_H
#define GURUTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>

class gurutcp : public QObject
{
    Q_OBJECT

public:
    explicit gurutcp(QObject *parent = 0);
    unsigned short setpost;
    QHostAddress ipaddress;
    QTcpServer* server;
    QTcpSocket* socket;
    QByteArray store;
    QString verifypass;

public slots:
    void newConnection();
    void start_connection();
    void disconnectserver();
    void readdata();
    void setXchange(int);
    void setYchange(int);
    void setZchange(int);

signals:
    void senddata();
};

#endif // GURUTCP_H
