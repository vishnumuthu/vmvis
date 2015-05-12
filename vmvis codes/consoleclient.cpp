#include "consoleclient.h"

consoleclient::consoleclient(QWidget *parent) :
    QPlainTextEdit(parent)
{
    // maximun count for the text
    document()->setMaximumBlockCount(100);
    // giving color for the terminal
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::red);
    setPalette(p);
}
// reading the data from the sensor and display in the text.
void consoleclient::putData(const QByteArray &data)
{

    QString value = data;
    // public slot of qplaintextedit
    insertPlainText(value);
    insertPlainText("\n");
    // creating the vertical scrollbar to maximum.
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}
// stop use of the keys.
void consoleclient::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}
// events to be stoped from use.
void consoleclient::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void consoleclient::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void consoleclient::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}

// print the sensor data
void consoleclient::print()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("Sensordata.pdf");
    QString buffer = this->document()->toHtml();
    this->setPlainText(this->toPlainText());
    this->document()->print(&printer);
    this->appendHtml(buffer);
}

