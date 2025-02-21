#pragma once

#include <QEvent>

namespace playlist_event
{
    enum Type
    {
        SaveEvent = QEvent::Type::User + 1,
        SaveAsEvent,
        CloseEvent,
        RejectEvent,
        LoadEvent,
    };
}
