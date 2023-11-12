cmake_minimum_required(VERSION 3.0)
project(warp-tray)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(APPINDICATOR REQUIRED appindicator3-0.1)

include_directories(${APPINDICATOR_INCLUDE_DIRS})

add_executable(warp-tray main.cpp)
target_include_directories(warp-tray PRIVATE ${GTK3_INCLUDE_DIRS} ${APPINDICATOR_INCLUDE_DIRS})
target_link_libraries(warp-tray PRIVATE ${GTK3_LIBRARIES} ${APPINDICATOR_LIBRARIES})
