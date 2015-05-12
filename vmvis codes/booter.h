#ifndef BOOTER_H
#define BOOTER_H

#include <QDialog>
#include "guruwindow.h"
#include "shishpass.h"

namespace Ui {
class booter;
}

class booter : public QDialog
{
    Q_OBJECT

public:
    explicit booter(QWidget *parent = 0);
    guruwindow *start_guru;
    shishpass start_shish;
    ~booter();

private slots:
    void on_bootguru_clicked();

    void on_bootshishya_clicked();

private:
    Ui::booter *ui;
};

#endif // BOOTER_H
