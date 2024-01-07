#pragma once

#include <QTreeView>
#include "filesystem_model.h"

#include <filesystem>

namespace fs = std::filesystem;

class FileSystemView : public QTreeView
{
public:
    FileSystemView(QWidget* parent = nullptr);
    FileSystemView(const fs::path& path, QWidget* parent = nullptr);

    const QString& ShortName() const;

private:
    void InitModel();

private:
    QString mShortName;
    FileSystemModel mModel;
};
