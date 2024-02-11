#include "filesystem_view.h"

#include <QDebug>

FileSystemView::FileSystemView(QWidget* parent)
        : QTreeView(parent)
{
    setStyleSheet("background-color:white;color:black;");
    static int newPlaylistCnt = 0;
    mShortName = QString("NewPlaylist%1").arg(++newPlaylistCnt);
    InitModel();
}

FileSystemView::FileSystemView(const fs::path& path, QWidget* parent)
    : QTreeView(parent)
    , mShortName(path.filename().c_str())
{
    InitModel();
}

void FileSystemView::InitModel()
{
    qDebug() << QDir::home().path();
    mModel.setRootPath(QDir::home().path());
    setModel(&mModel);
    setCurrentIndex(mModel.index(QDir::home().path()));

    for(int i = 1; i < mModel.columnCount(); ++i) {
        hideColumn(i);
    }
}

const QString& FileSystemView::ShortName() const
{
    return mShortName;
}
