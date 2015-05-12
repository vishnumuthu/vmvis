#ifndef GURUWINDOW_H
#define GURUWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

//tcpserver
#include "gurutcp.h"

//camera
#include <QCamera>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QCameraViewfinder>
#include <QMessageBox>

// sensor
#include "sensorgl.h"
#include "testgl.h"

// Terminal
#include <QtCore/QtGlobal>
#include <QtSerialPort/QSerialPort>
#include "sensorsetting.h"

namespace Ui {
class guruwindow;
}

class guruwindow : public QMainWindow
{
    Q_OBJECT
    QString password;

public:
    explicit guruwindow(QWidget *parent = 0);
    testGL testxyz;
    gurutcp start;
    ~guruwindow();

private slots:
    // TCP Server
    void on_serverconnect_clicked();
    void on_serverdisconnect_clicked();
    void on_allportandip_toggled(bool checked);
    void on_singleport_toggled(bool checked);
    void on_singleip_toggled(bool checked);
    void on_wordpass_editingFinished();
    void on_verifypass_editingFinished();
    void on_portinput_editingFinished();
    void on_ipinput_editingFinished();

    //camera
    void setCamera(const QByteArray &cameraDevice);
    void displayCameraError();
    void on_startcam_clicked();
    void on_stopcam_clicked();
    void on_refresh_clicked();
    void on_cameratype_currentIndexChanged(int index);

    //sensor
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void on_sensorconnect_clicked();
    void on_sensordisconnect_clicked();
    void on_testglxyz_clicked();
    void on_selectsensor_currentIndexChanged(int index);

    //Terminal
    void on_serialconnect_clicked();
    void on_serialdisconnect_clicked();
    void on_sensorsetup_clicked();

    // Action
    void on_actionClose_triggered();
    void on_actionTest_Setup_triggered();
    void on_actionAbout_Ti_Chronos_triggered();
    void on_actionCom_Port_triggered();
    void on_actionAbout_V_M_V_I_S_triggered();
    void on_actionHelp_triggered();

signals:
    // sensor
    void outputx(int);
    void outputy(int);
    void outputz(int);

public slots:
    // TCP Server
    void acceptdata();

    // Terminal
    void opencontrol();

private:
    Ui::guruwindow *ui;
    //camera
    QCamera *camera;

    //Terminal
    sensorsetting *settings;
    QSerialPort *serial;


};

#endif // GURUWINDOW_H
