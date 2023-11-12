cmake_minimum_required(VERSION 3.5)

project(nc-toggle-tray)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(nc-toggle-tray main.cpp)

target_link_libraries(nc-toggle-tray PRIVATE Qt5::Widgets)

