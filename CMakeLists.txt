cmake_minimum_required(VERSION 3.17)
project(Path_Finding_Algorithms_On_Graphs)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

add_executable(Path_Finding_Algorithms_On_Graphs
        src/main.cpp
        src/edge.cpp
        src/graphwidget.cpp
        src/node.cpp
        src/mainwindow.cpp
        src/rightmenu.cpp
        src/resources/resources.qrc src/algorithms.cpp src/algorithms.h)

target_link_libraries(Path_Finding_Algorithms_On_Graphs Qt5::Widgets Qt5::Core Qt5::Gui)