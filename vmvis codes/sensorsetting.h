#ifndef SENSORSETTING_H
#define SENSORSETTING_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class sensorsetting;
}

class QIntValidator;

QT_END_NAMESPACE

class sensorsetting : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };
    explicit sensorsetting(QWidget *parent = 0);
    ~sensorsetting();

    Settings settings() const;

private:
    Ui::sensorsetting *ui;
    Settings currentSettings;
    QIntValidator *intValidator;

    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

signals:
    void control();

private slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
};

#endif // SENSORSETTING_H
