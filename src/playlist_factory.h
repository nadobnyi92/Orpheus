#pragma once

#include <memory>

#include <QString>
#include <QStringList>

#include "iplaylist.h"

class PlaylistFactory
{
public:
    PlaylistFactory();

    std::unique_ptr< IPlaylistStorage > createStorage( const QString& path ) const;
    QString playlistFilterString() const;
    QStringList getNames() const;
    QList< QStringList> getExtensions() const;

private:
    QList< IPlaylistDescription* > mPlaylists;
};

