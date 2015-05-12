#ifndef TESTGL_H
#define TESTGL_H

#include <QDialog>

namespace Ui {
class testGL;
}

class testGL : public QDialog
{
    Q_OBJECT

public:
    explicit testGL(QWidget *parent = 0);
    ~testGL();
signals:
    void sendx(int);
    void sendy(int);
    void sendz(int);

private slots:
    void on_slidexdir_valueChanged(int value);
    void on_slideydir_valueChanged(int value);
    void on_slidezdir_valueChanged(int value);

private:
    Ui::testGL *ui;
};

#endif // TESTGL_H
