#include "basic.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Basic* basic = new Basic();
    basic->run();

    return a.exec();
}
