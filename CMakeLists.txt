project(netcatpaste)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(QCA2 REQUIRED)

link_directories( ${QCA2_LIBRARY_DIRS} )
include_directories( ${QCA2_INCLUDE_DIR} )
add_definitions( -DQCA2 )

#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${Qt5Widgets_QT_INCLUDES})
#include_directories(${Qt5Network_QT_INCLUDES})

#add_definitions(${Qt5Widgets_DEFINITIONS})
#add_definitions(${Qt5Network_DEFINITIONS})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}
#    ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

set(netcatpaste_SRCS netcatclient.cpp netcatpaste.cpp main.cpp)

add_executable(netcatpaste ${netcatpaste_SRCS})

#qt5_use_package(netcatpaste Widgets Network)
target_link_libraries(netcatpaste Qt5::Widgets Qt5::Network ${QCA2_LIBRARIES})
