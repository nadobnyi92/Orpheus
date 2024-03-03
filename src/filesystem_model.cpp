#include "filesystem_model.h"

#include <QStringList>

#include <iostream>
#include <qabstractitemmodel.h>

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
        if(  startedIt != mCheckedNodes.end() ) {
            return Qt::Checked;
        }
        auto partionStartedIt = std::find_if( mCheckedNodes.begin(), mCheckedNodes.end(),
                [ &path ]( const QString&  selectedPath ) {
                    return selectedPath.startsWith( path );
                } );
        if( partionStartedIt != mCheckedNodes.end() ) {
            return Qt::PartiallyChecked;
        }
        return Qt::Unchecked;
	}
    return QFileSystemModel::data(index, role);
}

bool FileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole && index.column() == 0) {
        if( value.toInt() == Qt::Checked ) {
            mCheckedNodes.append( filePath( index ) );
            QModelIndex tmpIndex = index.parent();
            while( mergeNode( tmpIndex ) && tmpIndex.isValid() ) {
                tmpIndex = tmpIndex.parent();
            }
        } else {
            QModelIndex tmpIndex = index.parent();
            while( unmergeNode( tmpIndex ) && tmpIndex.isValid() ) {
                tmpIndex = index.parent();
            }
            mCheckedNodes.removeAll( filePath( index ) );
        }
		return true;
	}
    return QFileSystemModel::setData(index, value, role);
}

Qt::ItemFlags FileSystemModel::flags(const QModelIndex &index) const
{
    if( index.column() == 0 ) {
        return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable;
    }
    return QFileSystemModel::flags(index);
}

bool FileSystemModel::mergeNode( const QModelIndex& parent )
{
    QStringList subPaths {};
    for( int i = 0; i < rowCount( parent ); ++i ) {
        subPaths.append( filePath( index( i, 0, parent ) ) );
    }
    bool isNeedMerge = std::all_of( mCheckedNodes.begin(), mCheckedNodes.end(), 
        [ &subPaths ]( const QString& path ) {
            return subPaths.contains( path );
        } );
    if( isNeedMerge ) {
        for( auto subPath: subPaths ) {
            mCheckedNodes.removeAll( subPath );
        }
        mCheckedNodes.append( filePath( parent ) );
        return true;
    }

    return false;
}

bool FileSystemModel::unmergeNode( const QModelIndex& parent )
{
    QString path = filePath( parent );
    auto partionStartedIt = std::find_if( mCheckedNodes.begin(), mCheckedNodes.end(),
            [ &path ]( const QString&  selectedPath ) {
                return path.startsWith( selectedPath );
            } );
    if( partionStartedIt == mCheckedNodes.end() ) {
        return false;
    }

    for( int i = 0; i < rowCount( parent ); ++i ) {
        mCheckedNodes.append( filePath( index( i, 0, parent ) ) );
    }
    mCheckedNodes.removeAll( path );
    return true;
}

