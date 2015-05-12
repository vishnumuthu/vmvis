#include <QApplication>
#include "booter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // create object for booter class
    booter start;
    // start the application
    start.show();
    return a.exec();
}
