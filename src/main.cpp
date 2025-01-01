#include "orpheus.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Orpheus w;
    w.show();
    return a.exec();
}
