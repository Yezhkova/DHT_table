#pragma once

#include <functional>

// comment/uncomment define to switch
#define PRIORITY_QUEUE

#ifdef PRIORITY_QUEUE
#include <queue>

class EventQueue
{
public:
    using Timestamp = double;
    using Interval = double;

private:
    struct Event
    {
        Timestamp              m_time;
        std::function<void()>  m_task = {};

        friend bool operator< (Event const& lhs, Event const& rhs) {
            return rhs.m_time < lhs.m_time;
        }
    };

    std::priority_queue<Event> m_queue;
    Timestamp                  m_queueCurrentTime = 0;

public:
    EventQueue() {}
    void addTaskAt(Interval delay, std::function<void()>&& task = {});
    void run();
    void removeAllEvents();

    Timestamp currentTime() const { return m_queueCurrentTime; }
};

#else

class EventQueue
{
public:
    using Timestamp = double;
    using Interval = double;

private:
    struct Event
    {
        //std::string            m_dbgLabel;
        Event*                 m_next = nullptr;
        Timestamp              m_time;
        std::function<void()>  m_task = {};
    };

    Event*          m_head; // NOT the first event, but a POINTER to the first event
    Timestamp       m_queueCurrentTime = 0;

public:
    EventQueue() {}
    Timestamp currentTime() const {
        return m_queueCurrentTime;
    }
    void addTaskAt(Timestamp t, std::function<void()> task = {});
    void run();
    void setEndTime(Timestamp time);
    void removeAllEvents();
};

#endif
