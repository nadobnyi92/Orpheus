#include "playlist_m3u8.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

PlaylistM3U8Storage::PlaylistM3U8Storage(QString path)
    : mPath(std::move(path))
{
}

const QStringList& PlaylistM3U8Storage::files() const
{
    return mFiles;
}

std::error_code PlaylistM3U8Storage::save(const QStringList& files)
{
    mFiles = files;
    QFile file{ mPath };
    QDir playlistDir = QFileInfo{ mPath }.absoluteDir();
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QMessageBox::critical(nullptr, "failed save playlist", QString{ "Failed create file %1" }.arg(mPath));
    }
    QTextStream playlistStream{ &file };
    playlistStream << "#EXTM3U\n\n";
    for(const auto& filename: mFiles )
    {
        playlistStream << "#EXTM3U," << QFileInfo{ filename }.fileName() << "\n";
        playlistStream << playlistDir.relativeFilePath( filename ) << "\n\n";
    }

    file.close();
    return {};
}

std::error_code PlaylistM3U8Storage::load()
{
    /// TODO load from files
    return {};
}

const QString& PlaylistM3U8Description::name() const
{
    static const QString name_ { "M3U playlist format" };
    return name_;
}

const QStringList& PlaylistM3U8Description::extensions() const
{
    static const QStringList extensions_{ "m3u", "m3u8" };
    return extensions_;
}

IPlaylistStorage* PlaylistM3U8Description::createStorage(const QString& path)
{
    return new PlaylistM3U8Storage(path);
}
