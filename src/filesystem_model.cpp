#include "filesystem_model.h"

#include <QStringList>

#include <iostream>

FileSystemModel::FileSystemModel(QObject* parent)
    : QFileSystemModel(parent)
{
    static const QStringList AudioExtensions {
        "*.mp3",
        "*.flac",
        "*.wav"
    };

    setNameFilters(AudioExtensions);
    setNameFilterDisables(false);
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::CheckStateRole && index.column() == 0) {
        QString path = filePath( index );
        auto startedIt = std::find_if( mCheckedNodes.begin(), mCheckedNodes.end(),
                [ &path ]( const QString&  selectedPath ) {
                    return path.startsWith( selectedPath );
                } );
        if(  startedIt != mCheckedNodes.end() )
        {
            return Qt::Checked;
        }
        auto partionStartedIt = std::find_if( mCheckedNodes.begin(), mCheckedNodes.end(),
                [ &path ]( const QString&  selectedPath ) {
                    return selectedPath.startsWith( path );
                } );
        
        return mCheckedNodes.contains( path ) ? Qt::Checked : Qt::Unchecked;
	}
    return QFileSystemModel::data(index, role);
}

bool FileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole && index.column() == 0) {
        mCheckedNodes.append( filePath( index ) );
		return true;
	}
    return QFileSystemModel::setData(index, value, role);
}

Qt::ItemFlags FileSystemModel::flags(const QModelIndex &index) const
{
    return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
}

