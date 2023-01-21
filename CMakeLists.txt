cmake_minimum_required(VERSION 3.14)

project( LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE main.cpp
           ID.h                             ID.cpp
           Utils.h                          Utils.cpp
           Node.h                           Node.cpp
           NodeInfo.h                       NodeInfo.cpp
           Bucket.h                         Bucket.cpp
           BucketMap.h                      BucketMap.cpp
           Peer.h                           Peer.cpp
           SwarmSimulator.h                 SwarmSimulator.cpp
           Swarm.h                          Swarm.cpp
           ISimulationTask.h                ISimulationTask.cpp
           BootstrapNode.h                  BootstrapNode.cpp
           IKademliaTransportProtocol.h     IKademliaTransportProtocol.cpp
           SimulationKtp.h                  SimulationKtp.cpp
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(kademlia ${SOURCE})

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost COMPONENTS)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(kademlia ${Boost_LIBRARIES})
endif()

#find_package(Boost 1.81.0)
#if(Boost_FOUND)
#  include_directories(${Boost_INCLUDE_DIRS})
#endif()

install (TARGETS kademlia
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
