#pragma once

#include <QtWidgets/QMainWindow>

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

    void createPlaylist(int idx) const;
    void onPlaylistChanged(int idx) const;
    void onPlaylistClose(int idx) const;

private:
    Ui::OrpheusClass mUi;
};
