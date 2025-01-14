#include "orpheus.h"

#include <QTabBar>
#include <QTreeView>

#include "playlistmodel.h"

Orpheus::Orpheus(QWidget* parent)
    : QMainWindow(parent)
{
    mUi.setupUi(this);
    mUi.twPlaylists->setTabsClosable(true);
    mUi.twPlaylists->tabBar()->tabButton(0, QTabBar::RightSide)->hide();
    createPlaylist(0);

    connect(mUi.twPlaylists, &QTabWidget::currentChanged,
        this, &Orpheus::onPlaylistChanged);

    connect(mUi.twPlaylists, &QTabWidget::tabCloseRequested,
        this, &Orpheus::onPlaylistClose);
}

void Orpheus::createPlaylist(const int idx) const
{
    static int newPlaylistIdx = 0;
    const QString newTitle = QString { "New Playlist %1" }.arg(++newPlaylistIdx);
    const auto view = new QTreeView(mUi.twPlaylists);
    const auto model = new PlaylistModel(view);
    view->setModel(model);
    view->setHeaderHidden(true);
    view->setCurrentIndex(model->index(QDir::homePath()));
    const int newTabIndex = mUi.twPlaylists->insertTab(idx, view, newTitle);
    mUi.twPlaylists->setCurrentIndex(newTabIndex);
}

void Orpheus::onPlaylistChanged(const int idx) const
{
    if (const int lastIdx = mUi.twPlaylists->count() - 1; idx == lastIdx) {
        createPlaylist(lastIdx);
    }
}

void Orpheus::onPlaylistClose(const int idx) const
{
    mUi.twPlaylists->removeTab(idx);
}
