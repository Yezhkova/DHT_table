cmake_minimum_required(VERSION 3.14)

project( KADEMLIA_DHT CXX)

if (UNIX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
endif (UNIX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE main.cpp
           ID.h                        ID.cpp
           Utils.h                     Utils.cpp
           Node.h                      Node.cpp
           NodeInfo.h                  NodeInfo.cpp
           BucketMap.h                 BucketMap.cpp
           Peer.h                      Peer.cpp
           Swarm.h                     Swarm.cpp
           ISimulationTask.h           ISimulationTask.cpp
           IDhtTransportProtocol.h     IDhtTransportProtocol.cpp
           Contact.h                   Contact.cpp
           INodeEventHandler.h         INodeEventHandler.cpp
           EventQueue.h                EventQueue.cpp
           PeerStatistics.h            PeerStatistics.cpp
           NodeRef.h                   NodeRef.cpp
   )

if (UNIX)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
endif (UNIX)

add_executable(kademlia ${SOURCE})

if (WIN32)
#set(BOOST_ROOT "C:/Program Files/boost/boost_1_81_0")
#set(BOOST_LIBRARYDIR "C:/Program Files/boost/boost_1_81_0/stage/lib")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,100000000")
endif (WIN32)
if (UNIX)
#set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
#set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
endif (UNIX)

#set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)

#find_package(Boost COMPONENTS chrono)

#if(Boost_FOUND)
  #include_directories(${Boost_INCLUDE_DIRS})
  #target_link_libraries(kademlia ${Boost_LIBRARIES})
#endif()

install (TARGETS kademlia
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
