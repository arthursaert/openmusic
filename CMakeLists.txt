cmake_minimum_required(VERSION 3.20)
project(OpenMusic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

add_executable(openmusic
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
)

target_link_libraries(openmusic
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
)