#pragma once

#include <QFileSystemModel>
#include <qfilesystemmodel.h>

class FileSystemModel : public QFileSystemModel
{
public:
    FileSystemModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    ///@brief попытаться объекдинить узлы в path
    ///@return попытаться объекдинить узлы в path
    bool mergeNode( const QModelIndex& index );

    ///@brief попытаться разбить узлы до path
    ///@return попытаться объекдинить узлы в path
    bool unmergeNode( const QModelIndex& index );

private:
    QStringList mCheckedNodes;
};
