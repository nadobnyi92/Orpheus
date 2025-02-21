#pragma once

#include <system_error>

#include <QString>

class IPlaylistStorage
{
public:
    virtual ~IPlaylistStorage() = default;

    virtual const QStringList& files() const = 0;

    virtual std::error_code save(const QStringList& files) = 0;
    virtual std::error_code load() = 0;
};

class IPlaylistDescription
{
public:
    virtual ~IPlaylistDescription() = default;

    virtual const QString& name() const = 0;
    virtual const QStringList& extensions() const = 0;

    virtual IPlaylistStorage* createStorage(const QString& path) = 0;
};


