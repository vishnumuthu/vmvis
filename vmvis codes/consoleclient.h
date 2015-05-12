#ifndef CONSOLECLIENT_H
#define CONSOLECLIENT_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QtCore/QDebug>
#include "qprinter.h"

class consoleclient : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit consoleclient(QWidget *parent = 0);
    void putData(const QByteArray &data);

public slots:
    void print();

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);


};

#endif // CONSOLECLIENT_H
