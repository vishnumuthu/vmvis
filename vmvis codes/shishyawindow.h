#ifndef SHISHYAWINDOW_H
#define SHISHYAWINDOW_H

#include <QMainWindow>
#include "qhostaddress.h"

namespace Ui {
class shishyawindow;
}

class shishyawindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit shishyawindow(QWidget *parent = 0);
    QString ipvalue;
    QString portvalue;
    ~shishyawindow();

signals:
    void setXvaluex(int);
    void setYvaluey(int);
    void setZvaluez(int);
    void disconnecttrig();

public slots:
    void setX(int value);
    void setY(int value);
    void setZ(int value);
    void setvalue();
    void getsensor(QByteArray);

private slots:
    void on_disconnect_clicked();

    void on_actionClose_triggered();

    void on_actionAbout_vmvis_V1_1_triggered();

    void on_actionHelp_triggered();

private:
    Ui::shishyawindow *ui;
};

#endif // SHISHYAWINDOW_H
