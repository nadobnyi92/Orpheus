#pragma once

#include <QTreeView>

class PlaylistView : public QTreeView
{
    Q_OBJECT

public:
    explicit PlaylistView(QWidget *parent);

    void initModel(const QString& title);

signals:
    void stateChanged(const QString& title);

protected:
    bool event(QEvent* event) override;

    ~PlaylistView() override = default;
};
