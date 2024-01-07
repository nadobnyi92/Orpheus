#pragma once

#include "ui_mainwindow.h"
#include "filesystem_view.h"

#include <memory>

class MainWindow : public QMainWindow
{
public:
    MainWindow( QWidget* parent = nullptr );

    void addTab( int index, FileSystemView* parent );
    void dropTab( int index );

private:
    std::unique_ptr< Ui::MainWindow > mUi;
};
