#include "EventQueue.h"

/*
struct Event
{
    Event*                 m_next = nullptr;
    Timestamp              m_eventTime;
    std::function<void()>  m_task = {};
};

Event           m_head;      NOT the first event, but a POINTER to the first event
Timestamp       m_currentTime = 0;
*/

EventQueue::Timestamp EventQueue::addTask(Duration deltaTime, std::function<void()> task) {
    Timestamp t = m_currentTime + deltaTime;
    auto* event = &m_head;
    for(; event->m_next != nullptr; event = event->m_next) {
        if(event->m_next->m_eventTime > t) {
            auto* e = new Event{event->m_next, t, task};
            event->m_next = e;
        }
    }
    return t;
}

void EventQueue::run() {
    while(m_head.m_next != nullptr) {
        auto* e = m_head.m_next;
        m_head.m_next = e->m_next;
        e->m_task();
        delete e;
    }
}

void EventQueue::stop() {

}


