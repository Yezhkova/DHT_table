#include "EventQueue.h"
#include "Utils.h"
#include <ios>
#include <cassert>

#ifdef PRIORITY_QUEUE
void EventQueue::addTaskAt(Interval delay, std::function<void()> task)
{
    assert(m_queueCurrentTime <= delay);
    m_queue.emplace(Event {delay, task});
}

void EventQueue::run() {
    while(!m_queue.empty())
    {
        Event e = m_queue.top();
        m_queue.pop();
        assert(m_queueCurrentTime <= e.m_time);
        m_queueCurrentTime = e.m_time;
        e.m_task();
    }
}

void EventQueue::removeAllEvents() {
    while(!m_queue.empty()) {
        m_queue.pop();
    }
    m_queueCurrentTime = 0;
}

#else

void EventQueue::addTaskAt(Timestamp t, std::function<void()> task) {
    for (Event** ptr = &m_head; ; ptr = &(*ptr)->m_next)
    {
        if ((*ptr) == nullptr)
        {
            (*ptr) = new Event{ nullptr, t, task };
            return;
        }

        if ((*ptr)->m_time > t)
        {
            auto e = new Event{ (*ptr), t, task };
            (*ptr) = e;
            return;
        }
    }
    return;
}

void EventQueue::run() {
    while(m_head != nullptr) {
        auto* e = m_head;
        m_head = e->m_next;
        m_queueCurrentTime = e->m_time;
        e->m_task();
        delete e;
    }
}

void EventQueue::removeAllEvents() {
    while(m_head != nullptr) {
        auto* e = m_head;
        m_head = e->m_next;
        delete e;
    }
    m_queueCurrentTime = 0;
}

void EventQueue::setEndTime(Timestamp time) {
    addTaskAt(time, [this] {
        removeAllEvents();
    });
}

#endif


