cmake_minimum_required(VERSION 3.18)
project(Orpheus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(SRC src/main.cpp
        src/mainwindow.cpp
        src/filesystem_view.cpp
        src/filesystem_model.cpp
        src/mainwindow.ui
        )

add_executable(Orpheus ${SRC})

target_link_libraries(Orpheus Qt6::Core Qt6::Widgets)

file(CREATE_LINK ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json SYMBOLIC)

