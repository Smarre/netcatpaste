project(netcatpaste)
cmake_minimum_required(VERSION 2.6)

if(UNIX)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/share/apps/cmake/modules)
endif()

find_package(Qt4 REQUIRED)
find_package(QCA2 REQUIRED)
set(QT_USE_QTNETWORK true)

if ( QCA2_FOUND )
    link_directories( ${QCA2_LIBRARY_DIRS} )
    include_directories( ${QCA2_INCLUDE_DIR} )
    add_definitions( -DQCA2 )
endif()

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(netcatpaste_SRCS netcatpaste.cpp main.cpp)
qt4_automoc(${netcatpaste_SRCS})
add_executable(netcatpaste ${netcatpaste_SRCS})
target_link_libraries(netcatpaste ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QCA2_LIBRARIES})
