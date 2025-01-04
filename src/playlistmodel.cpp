#include "PlaylistModel.h"

namespace {
const QStringList AUDIO_EXTENSIONS = {
    "*.mp3",
    "*.flac",
    "*.wav"
};
}

PlaylistModel::PlaylistModel(QObject* parent)
    : QFileSystemModel(parent)
{
    setRootPath("");
    setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    setNameFilters(AUDIO_EXTENSIONS);
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
        if (isContainPath(filePath(index))) {
            deleteNode(index);
        } else {
            addNode(index);
        }
        updateParent(index);
        updateChildren(index);
        return true;
    }
    return QFileSystemModel::setData(index, value, role);
}

void PlaylistModel::addNode(const QModelIndex& index)
{
    const QString path = filePath(index);
    mCheckedPaths.append(path);
    if (!index.parent().isValid()) {
        return;
    }
    QStringList siblingNodes {};
    for (int row = 0; row < rowCount(index.parent()); ++row) {
        const QModelIndex childIndex = QFileSystemModel::index(row, 0, index.parent());
        QString childPath = filePath(childIndex);
        if (!mCheckedPaths.contains(childPath)) {
            return;
        }
        siblingNodes.append(childPath);
    }
    for (const auto& siblingPath : siblingNodes) {
        mCheckedPaths.removeAll(siblingPath);
    }

    addNode(index.parent());
}

void PlaylistModel::deleteNode(const QModelIndex& index)
{
    if (const QString path = filePath(index); mCheckedPaths.contains(path)) {
        mCheckedPaths.removeAll(path);
    } else if (index.parent().isValid()) {
        for (int row = 0; row < rowCount(index.parent()); ++row) {
            const QModelIndex childIndex = QFileSystemModel::index(row, 0, index.parent());
            if (const QString childFilePath = filePath(childIndex); childFilePath != path) {
                mCheckedPaths.append(childFilePath);
            }
        }
        deleteNode(index.parent());
    }
}

void PlaylistModel::updateParent(const QModelIndex& index)
{
    QModelIndex parent = index.parent();
    if (parent.isValid()) {
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
        return !QDir { selectPath }.relativeFilePath(path).startsWith("../");
    }) != mCheckedPaths.end();
}

bool PlaylistModel::isPartialContainPath(const QString& path) const
{
    return std::find_if(mCheckedPaths.begin(), mCheckedPaths.end(),
               [&path](const QString& selectPath) {
                   return !QDir { path }.relativeFilePath(selectPath).startsWith("../");
               })
        != mCheckedPaths.end();
}
