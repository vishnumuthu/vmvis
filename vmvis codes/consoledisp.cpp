#include "consoledisp.h"

consoledisp::consoledisp(QWidget *parent) :
    QPlainTextEdit(parent), localEchoEnabled(false)
{
    // maximun count for the text
    document()->setMaximumBlockCount(100);
    // giving color for the terminal
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
}
// reading the data from the sensor and display in the text.
void consoledisp::putData(const QByteArray &data)
{

    QString value;
    for(int i=0;i<data.size();i++)
    {
        quint8 out = 0;
        out |= data[i];
        value = QString::number(out);
        // public slot of qplaintextedit
        insertPlainText(value);
        insertPlainText("     ");
    }
    insertPlainText("\n");
    // creating the vertical scrollbar to maximum.
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}
// set local echo mode.
void consoledisp::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}
// stop use of the keys.
void consoledisp::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if (localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }
}
// events to be stoped from use.
void consoledisp::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void consoledisp::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void consoledisp::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
// print the sensor data
void consoledisp::print()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("Sensordata.pdf");
    QString buffer = this->document()->toHtml();
    this->setPlainText(this->toPlainText());
    this->document()->print(&printer);
    this->appendHtml(buffer);
}
