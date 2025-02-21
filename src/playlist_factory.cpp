#include "playlist_factory.h"

#include "playlist_m3u8.h"

#include <QFileInfo>

PlaylistFactory::PlaylistFactory()
{
    mPlaylists = {
        new PlaylistM3U8Description
    };
}

std::unique_ptr<IPlaylistStorage> PlaylistFactory::createStorage(const QString& path) const
{
    const QString extension = QFileInfo( path ).suffix();
    for( const auto& desc: mPlaylists )
    {
        if( desc->extensions().contains( extension ) )
        {
            return std::unique_ptr<IPlaylistStorage>(desc->createStorage(path));
        }
    }
    return nullptr;
}

QString PlaylistFactory::playlistFilterString() const
{
    QStringList filters{};
    for(const auto& desc: mPlaylists)
    {
        QStringList extensions = {};
        for( const auto& ext: desc->extensions())
        {
            extensions.append(QString("*.%1").arg(ext));
        }
        filters.append( 
            QString("%1 (%2)")
                .arg(desc->name())
                .arg(extensions.join(" "))
            );
    }
    return filters.join(";;");
}

QStringList PlaylistFactory::getNames() const
{
    QStringList names;
    for( const auto& desc: mPlaylists )
    {
        names.append(desc->name());
    }
    return names;
}

QList<QStringList> PlaylistFactory::getExtensions() const
{
    QList< QStringList > extensions;
    for( const auto& desc: mPlaylists )
    {
        extensions.append(desc->extensions());
    }
    return extensions;

}
