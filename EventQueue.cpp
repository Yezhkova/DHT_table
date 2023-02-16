#include "EventQueue.h"
#include <ios>
#include "Utils.h"
#include <cassert>
/*
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

*/

EventQueue::Timestamp EventQueue::addTask(Interval delay
                                          , std::function<void()> task) {
    Timestamp t = m_queueCurrentTime + delay;
    auto* event = &m_head;
    for(; event->m_next != nullptr; event = event->m_next) {
        if(event->m_eventTime <= t) {
            continue;
        }
        auto* e = new Event{event->m_next, t, task};
        event->m_next = e;
        return t;
    }
    assert(event->m_next == nullptr);
    auto* e = new Event{nullptr, t, task};
    event->m_next = e;
    return t;
}
void EventQueue::run() {
    while(m_head.m_next != nullptr) {
        auto* e = m_head.m_next;
        m_head.m_next = e->m_next;
        m_queueCurrentTime += e->m_eventTime;
        e->m_task();
        delete e;
    }
}

void EventQueue::stop() {
    while(m_head.m_next != nullptr) {
        auto* e = m_head.m_next;
        m_head.m_next = e->m_next;
        delete e;
    }
}

void EventQueue::setEndTime(Timestamp time) {
    addTask(time, [this] {
        stop();
    });
}


