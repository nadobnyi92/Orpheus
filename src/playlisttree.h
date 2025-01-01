#pragma once

#include <QTreeView>

class PlaylistTree : public QTreeView
{
    Q_OBJECT

public:
    PlaylistTree(QObject *parent);
    ~PlaylistTree();
};
