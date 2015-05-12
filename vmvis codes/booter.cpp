#include "booter.h"
#include "ui_booter.h"

booter::booter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::booter)
{
    ui->setupUi(this);
    // General specification for the guruwindow # Hide the full screen button and set the name for window and fixed size.
    this->setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle("V.M.V.I.S. Boot Window");
}

booter::~booter()
{
    delete ui;
}
// one the server section
void booter::on_bootguru_clicked()
{
    this->close();
    start_guru = new guruwindow;
    start_guru->show();

}
// open the client section
void booter::on_bootshishya_clicked()
{
    this->close();
    start_shish.show();
}

