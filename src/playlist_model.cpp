#include <QDirIterator>

#include "playlist_model.h"
#include "events.h"

#include <QFileDialog>
#include <QMessageBox>

namespace {
const QStringList AUDIO_EXTENSIONS = {
    "*.mp3",
    "*.flac",
    "*.wav"
};
}

static QString normalizePath( const QString& path )
{
    if( QFileInfo(path).isDir() && !path.endsWith('/'))
    {
        return path + '/';
    }
    else
    {
        return path;
    }
}

PlaylistModel::PlaylistModel(const QString& title, QObject* parent)
    : QFileSystemModel(parent)
    , mTitle(title)
    , mStorage(nullptr)
{
    setRootPath("");
    setNameFilters(AUDIO_EXTENSIONS);
    setNameFilterDisables(false);
}

int PlaylistModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex& index) const
{
    return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::CheckStateRole && index.column() == 0) {
        const QString path = filePath(index);
        if (isContainPath(path)) {
            return Qt::Checked;
        }
        if (isPartialContainPath(path)) {
            return Qt::PartiallyChecked;
        }
        return Qt::Unchecked;
    }
    return QFileSystemModel::data(index, role);
}

bool PlaylistModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::CheckStateRole) {
        if (data(index, role).toInt() != Qt::Unchecked) {
            mChanged = deleteNode(index);
        } else {
            mChanged = addNode(index);
        }
        updateParent(index);
        updateChildren(index);
        if(mChanged)
        {
            emit stateChanged(mTitle+"*");
        }
        return true;
    }
    return QFileSystemModel::setData(index, value, role);
}

bool PlaylistModel::addNode(const QModelIndex& index)
{
    const QString path = filePath(index);
    QDirIterator it(path, AUDIO_EXTENSIONS, QDir::NoFilter, QDirIterator::Subdirectories);
    if(!it.hasNext())
    {
        QMessageBox::warning(nullptr, "empty folder", "Selected folder dont contain audio files");
        return false;
    }
    while(it.hasNext())
    {
        mCheckedPaths.append(it.next());
    }
    return true;
}

bool PlaylistModel::deleteNode(const QModelIndex& index)
{
    const QString delPath = normalizePath( filePath(index) );
    const qsizetype delElements =  mCheckedPaths.removeIf([&delPath](const QString& path) { return path.startsWith(delPath); });
    return delElements > 0;
}

void PlaylistModel::updateParent(const QModelIndex& index)
{
    if (const QModelIndex parent = index.parent(); parent.isValid() && parent.parent().isValid()) {
        emit dataChanged(parent, parent, { Qt::CheckStateRole });
        updateParent(parent);
    }
}

void PlaylistModel::updateChildren(const QModelIndex& index)
{
    for (int childIdx = 0; childIdx < rowCount(index); ++childIdx) {
        QModelIndex childIndex = QFileSystemModel::index(childIdx, 0);
        emit dataChanged(childIndex, childIndex, { Qt::CheckStateRole });
        updateParent(childIndex);
    }
}

bool PlaylistModel::isContainPath(const QString& path) const
{
    return mCheckedPaths.contains(path) || std::find_if(mCheckedPaths.begin(), mCheckedPaths.end(), [&path](const QString& selectPath) {
        return normalizePath(path).startsWith(normalizePath(selectPath) );
    }) != mCheckedPaths.end();
}

bool PlaylistModel::isPartialContainPath(const QString& path) const
{
    return std::find_if(mCheckedPaths.begin(), mCheckedPaths.end(), [&path](const QString& selectPath) {
        return normalizePath(selectPath).startsWith(normalizePath(path ));
    }) != mCheckedPaths.end();
}

QStringList PlaylistModel::getSelectedFiles() const
{
    return mCheckedPaths;
}

bool PlaylistModel::event(QEvent* event)
{
    switch (event->type())
    {
    case playlist_event::SaveEvent:
        return mStorage ? savePlaylist() : saveNewPlaylist();
    case playlist_event::SaveAsEvent:
        return saveNewPlaylist();
    case playlist_event::RejectEvent:
        return rejectPlaylist();
    default:
        return QFileSystemModel::event(event);
    }
}

bool PlaylistModel::savePlaylist()
{
    mChanged = false;
    emit stateChanged(mTitle);
    return true;
}

bool PlaylistModel::saveNewPlaylist()
{
    QString playlistName = QFileDialog::getSaveFileName(nullptr, "Save new playlist",
        QDir::homePath(), mPlaylistFactory.playlistFilterString());
    mStorage = mPlaylistFactory.createStorage(playlistName);
    if(mStorage)
    {
        mStorage->save(getSelectedFiles());
    }
    mChanged = false;
    emit stateChanged(mTitle);
    return true;
}

bool PlaylistModel::loadPlaylist()
{
    return true;
}

bool PlaylistModel::rejectPlaylist()
{
    if(!mChanged)
    {
        return false;
    }
    auto res = QMessageBox::question(nullptr, "Reject changes", "are you shure???");
    if(res != QMessageBox::Yes)
    {
        return false;
    }
    mCheckedPaths = mStorage ? mStorage->files() : QStringList{};
    mChanged = false;
    emit stateChanged(mTitle);
    emit dataChanged(QModelIndex(), QModelIndex());
    return true;
}


