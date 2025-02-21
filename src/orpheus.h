#pragma once

#include <QtWidgets/QMainWindow>

#include "events.h"
#include "ui_orpheus.h"

class Orpheus final : public QMainWindow {
    Q_OBJECT

public:
    explicit Orpheus(QWidget* parent = nullptr);
    Orpheus(const Orpheus&) = delete;
    Orpheus& operator=(const Orpheus&) = delete;
    Orpheus(const Orpheus&&) = delete;
    Orpheus& operator=(const Orpheus&&) = delete;
    ~Orpheus() override = default;

private:
    void createPlaylist(int idx) const;
    void onCurrentPlaylistChanged(int idx) const;
    void onPlaylistStateChanged(const QString& title);
    void onPlaylistClose(int idx) const;


    void onPlaylistAction(playlist_event::Type event) const;

private:
    Ui::OrpheusClass mUi;
};
