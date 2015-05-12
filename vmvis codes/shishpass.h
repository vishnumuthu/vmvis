#ifndef SHISHPASS_H
#define SHISHPASS_H

#include <QDialog>
#include "shishyawindow.h"
#include <QTcpSocket>
#include <QMessageBox>
#include "qhostaddress.h"

namespace Ui {
class shishpass;
}

class shishpass : public QDialog
{
    Q_OBJECT

public:
    explicit shishpass(QWidget *parent = 0);
    shishyawindow start_shishya;
    QHostAddress ipreq;
    ~shishpass();
public slots:
    void readresponse();
    void disconnectclient();

private slots:
    void on_clientconnect_clicked();
    void on_ipreq_editingFinished();
    void on_portreq_editingFinished();
    void on_passreq_editingFinished();

signals:
    void sendX(int);
    void sendY(int);
    void sendZ(int);
    void sensordata(QByteArray);

private:
    Ui::shishpass *ui;
    QTcpSocket *socket;
};

#endif // SHISHPASS_H
