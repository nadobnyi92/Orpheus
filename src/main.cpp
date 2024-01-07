#include "mainwindow.h"

#include <iostream>
#include <qguiapplication.h>

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    MainWindow window;
    window.show();
    return app.exec();
}
