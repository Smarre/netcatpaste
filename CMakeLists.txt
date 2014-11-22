project(netcatpaste)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(QCA2 REQUIRED)

link_directories( ${QCA2_LIBRARY_DIRS} )
include_directories( ${QCA2_INCLUDE_DIR} )
add_definitions( -DQCA2 )

set(netcatpaste_SRCS netcatclient.cpp netcatpaste.cpp main.cpp)

add_executable(netcatpaste ${netcatpaste_SRCS})

target_link_libraries(netcatpaste Qt5::Core Qt5::Network ${QCA2_LIBRARIES})
