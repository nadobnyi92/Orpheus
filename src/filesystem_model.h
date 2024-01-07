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
    QStringList mCheckedNodes;
};
