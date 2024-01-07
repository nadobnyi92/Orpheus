#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);
    mUi->twPlaylists->setTabsClosable(true);
    mUi->twPlaylists->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
    addTab(0, new FileSystemView(mUi->twPlaylists));

    connect( mUi->twPlaylists, &QTabWidget::currentChanged, 
             [this](int index){
                if(index == mUi->twPlaylists->count() - 1) {
                    addTab(index, new FileSystemView(mUi->twPlaylists));
                }
             } );

    connect( mUi->twPlaylists, &QTabWidget::tabCloseRequested,
             [this](int index){ dropTab(index); });
}

void MainWindow::addTab(int index, FileSystemView* view)
{
    mUi->twPlaylists->insertTab(index, view, {}, view->ShortName());
    mUi->twPlaylists->setCurrentIndex(index);
}

void MainWindow::dropTab(int index)
{
    mUi->twPlaylists->removeTab(index);
}
