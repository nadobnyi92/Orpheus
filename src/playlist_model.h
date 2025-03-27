#pragma once

#include <QFilesystemModel>

#include "playlist_factory.h"

class PlaylistModel final : public QFileSystemModel {
    Q_OBJECT

public:
    explicit PlaylistModel(const QString& title, QObject* parent);
    PlaylistModel(const PlaylistModel& model) = delete;
    PlaylistModel& operator=(const PlaylistModel&) = delete;
    PlaylistModel(PlaylistModel&&) = delete;
    PlaylistModel& operator=(PlaylistModel&&) = delete;

    ~PlaylistModel() override = default;

    int columnCount(const QModelIndex& parent) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    bool event(QEvent* event) override;

signals:
    void stateChanged(const QString& title);

private:
    bool addNode(const QModelIndex& index);
    bool deleteNode(const QModelIndex& index);

    void updateParent(const QModelIndex& index);
    void updateChildren(const QModelIndex& index);

    bool isContainPath(const QString& path) const;
    bool isPartialContainPath(const QString& path) const;

    QStringList getSelectedFiles() const;

    bool savePlaylist();
    bool saveNewPlaylist();
    bool loadPlaylist();
    bool rejectPlaylist();

private:
    QStringList mCheckedPaths;
    PlaylistFactory mPlaylistFactory;
    std::unique_ptr< IPlaylistStorage > mStorage;
    QString mTitle;
    bool mChanged;
};
