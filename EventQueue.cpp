#include "EventQueue.h"
#include <ios>
#include <cassert>
#include "Utils.h"

EventQueue::Timestamp EventQueue::addTaskAt(Interval delay
                                          , std::function<void()> task) {
    Timestamp t = m_queueCurrentTime + delay;
    for (Event** ptr = &m_head; ; ptr = &(*ptr)->m_next)
    {
        if ((*ptr) == nullptr)
        {
            (*ptr) = new Event{ nullptr, delay, task };
            return t;
        }

        if ((*ptr)->m_time > t)
        {
            auto e = new Event{ (*ptr), delay, task };
            (*ptr) = e;
            return t;
        }
    }
    return t;
}

void EventQueue::run() {
    while(m_head != nullptr) {
        auto* e = m_head;
        m_head = e->m_next;
        m_queueCurrentTime += e->m_time;
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
}

void EventQueue::setEndTime(Timestamp time) {
    addTaskAt(time, [this] {
        removeAllEvents();
    });
}



