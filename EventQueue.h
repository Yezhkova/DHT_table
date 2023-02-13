#pragma once

#include <functional>
class EventQueue
{
public:
    using Timestamp = double;
    using Duration = double;

private:
    struct Event
    {
        Event*                 m_next = nullptr;
        Timestamp              m_eventTime;
        std::function<void()>  m_task = {};
    };

    Event           m_head; // NOT the first event, but a POINTER to the first event
    Timestamp       m_currentTime = 0;

public:
    EventQueue() {}
    Timestamp currentTime() {
        return m_currentTime;
    }
    Timestamp addTask(Duration deltaTime = 0, std::function<void()> task = {});
    void run();
    void stop();
};

