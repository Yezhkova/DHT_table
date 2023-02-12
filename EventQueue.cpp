#include "EventQueue.h"

EventQueue::Timestamp EventQueue::addTask(Duration deltaTime, std::function<void()> task)
{
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

void EventQueue::start()
{
    while(m_head.m_next != nullptr) {
        auto* e = m_head.m_next;
        m_head.m_next = e->m_next;
        e->m_task();
        delete e;
    }
}
