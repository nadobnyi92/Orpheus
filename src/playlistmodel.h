#pragma once

#include <QFilesystemModel>

class PlaylistModel final : public QFileSystemModel {
    Q_OBJECT

public:
    explicit PlaylistModel(QObject* parent);
    PlaylistModel(const PlaylistModel& model) = delete;
    PlaylistModel& operator=(const PlaylistModel&) = delete;
    PlaylistModel(PlaylistModel&&) = delete;
    PlaylistModel& operator=(PlaylistModel&&) = delete;

    ~PlaylistModel() override = default;

    int columnCount(const QModelIndex& parent) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

private:
    void addNode(const QModelIndex& index);
    void deleteNode(const QModelIndex& index);

    void updateParent(const QModelIndex& index);
    void updateChildren(const QModelIndex& index);

    bool isContainPath(const QString& path) const;
    bool isPartialContainPath(const QString& path) const;

private:
    QStringList mCheckedPaths;
};
