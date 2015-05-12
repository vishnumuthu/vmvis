#ifndef CONSOLEDISP_H
#define CONSOLEDISP_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QtCore/QDebug>
#include "QtPrintSupport/qprinter.h"

class consoledisp : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit consoledisp(QWidget *parent = 0);
    void putData(const QByteArray &data);
    void setLocalEchoEnabled(bool set);

signals:
    void getData(const QByteArray &data);

public slots:
    void print();

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);

private:
    bool localEchoEnabled;

};

#endif // CONSOLEDISP_H
