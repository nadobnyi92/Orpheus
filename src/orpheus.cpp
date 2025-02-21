#include "orpheus.h"

#include <QTabBar>

#include "playlist_view.h"
#include "events.h"

Orpheus::Orpheus(QWidget* parent)
    : QMainWindow(parent)
{
    mUi.setupUi(this);
    mUi.twPlaylists->setTabsClosable(true);
    mUi.twPlaylists->tabBar()->tabButton(0, QTabBar::RightSide)->hide();
    createPlaylist(0);

    connect(mUi.twPlaylists, &QTabWidget::currentChanged,
        this, &Orpheus::onCurrentPlaylistChanged);

    connect(mUi.twPlaylists, &QTabWidget::tabCloseRequested,
        this, &Orpheus::onPlaylistClose);

    connect(mUi.actionSave, &QAction::triggered,
        this, [this]() {onPlaylistAction(playlist_event::SaveEvent);});

    connect(mUi.actionSaveAs, &QAction::triggered,
        this, [this]() {onPlaylistAction(playlist_event::SaveAsEvent);});

    connect(mUi.actionReject, &QAction::triggered,
        this, [this]() {onPlaylistAction(playlist_event::RejectEvent);});

}

void Orpheus::createPlaylist(const int idx) const
{
    static int newPlaylistIdx = 0;
    const QString newTitle = QString { "New Playlist %1" }.arg(++newPlaylistIdx);
    const auto view = new PlaylistView(mUi.twPlaylists);
    view->initModel(newTitle);
    const int newTabIndex = mUi.twPlaylists->insertTab(idx, view, newTitle);
    mUi.twPlaylists->setCurrentIndex(newTabIndex);
    connect(view, &PlaylistView::stateChanged,
        this, &Orpheus::onPlaylistStateChanged);
}

void Orpheus::onCurrentPlaylistChanged(const int idx) const
{
    if (const int lastIdx = mUi.twPlaylists->count() - 1; idx == lastIdx) {
        createPlaylist(lastIdx);
    }
}

void Orpheus::onPlaylistStateChanged(const QString& title)
{
    QObject* sender = QObject::sender();
    for(int i = 0; i < mUi.twPlaylists->count(); ++i)
    {
        if(mUi.twPlaylists->widget( i ) == QObject::sender())
        {
            mUi.twPlaylists->setTabText(i, title);
            break;
        }
    }

}

void Orpheus::onPlaylistClose(const int idx) const
{
    mUi.twPlaylists->removeTab(idx);
}

void Orpheus::onPlaylistAction(playlist_event::Type event) const
{
    QApplication::sendEvent(mUi.twPlaylists->currentWidget(), new QEvent(static_cast<QEvent::Type>(event)));
}
