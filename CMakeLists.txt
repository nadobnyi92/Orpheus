cmake_minimum_required(VERSION 3.16)
project(Orpheus LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    src/main.cpp
    src/events.h
    src/orpheus.ui
    src/orpheus.h
    src/orpheus.cpp
    src/playlist_model.h
    src/playlist_model.cpp
    src/playlist_view.cpp
    src/playlist_view.cpp
    src/iplaylist.h
    src/playlist_factory.h
    src/playlist_factory.cpp
    src/playlist_m3u8.h
    src/playlist_m3u8.cpp
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

