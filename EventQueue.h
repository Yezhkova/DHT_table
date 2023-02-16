#pragma once

#include <functional>
class EventQueue
{
public:
    using Timestamp = double;
    using Interval = double;

private:
    struct Event
    {
        Event*                 m_next = nullptr;
        Timestamp              m_eventTime;
        std::function<void()>  m_task = {};
    };

    Event           m_head; // NOT the first event, but a POINTER to the first event
    Timestamp       m_queueCurrentTime = 0;

public:
    EventQueue() {}
    Timestamp currentTime() {
        return m_queueCurrentTime;
    }
    Timestamp addTask(Interval deltaTime = 0, std::function<void()> task = {});
    void run();
    void setEndTime(Timestamp time);
    void stop();
};

