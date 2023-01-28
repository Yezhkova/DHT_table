#pragma once

#include <boost/chrono.hpp>
#include <boost/asio.hpp>

class NodeInfo
{
private:
    boost::chrono::system_clock::time_point m_lastSeen;

public:
    NodeInfo() = delete;

    NodeInfo(boost::chrono::system_clock::time_point time)
        : m_lastSeen(time) {};

};

