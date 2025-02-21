#pragma once

#include "iplaylist.h"

#include <QStringList>

class PlaylistM3U8Storage : public IPlaylistStorage
{
public:
    explicit PlaylistM3U8Storage(QString path);

    ~PlaylistM3U8Storage() override = default;

    const QStringList& files() const override;

    std::error_code save(const QStringList& files) override;
    std::error_code load() override;

private:
    QString mPath;
    QStringList mFiles;
};

class PlaylistM3U8Description : public IPlaylistDescription
{
public:
    ~PlaylistM3U8Description() override = default;

    const QString& name() const override;
    const QStringList& extensions() const override;

    IPlaylistStorage* createStorage(const QString& path) override;
};


