cmake_minimum_required(VERSION 3.14)

project( LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(dht_table main.cpp)

message(STATUS "bOOST include dir: ${Boost_INCLUDE_DIR}")


set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost COMPONENTS chrono)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(dht_table ${Boost_LIBRARIES})
endif()

#find_package(Boost 1.81.0)
#if(Boost_FOUND)
#  include_directories(${Boost_INCLUDE_DIRS})
#endif()

install (TARGETS dht_table
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
