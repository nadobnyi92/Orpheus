#include "playlist_view.h"

#include <QApplication>
#include <QEvent>

#include "events.h"
#include "playlist_model.h"

PlaylistView::PlaylistView(QWidget *parent)
    : QTreeView(parent)
{
}

void PlaylistView::initModel(const QString& title)
{
    const auto model = new PlaylistModel(title,this);
    this->setModel(model);
    this->setHeaderHidden(true);
    this->setCurrentIndex(model->index(QDir::homePath()));
    connect(model, &PlaylistModel::stateChanged,
        this, &PlaylistView::stateChanged);
}

bool PlaylistView::event(QEvent* event)
{
    switch (event->type())
    {
    case playlist_event::SaveEvent:
    case playlist_event::SaveAsEvent:
    case playlist_event::RejectEvent:
        return QApplication::sendEvent(model(), event);
    default:
        return QTreeView::event(event);
    }
}

