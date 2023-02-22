#include "EventQueue.h"
#include <ios>
#include <cassert>
#include "Utils.h"

//unsigned int EventQueue::size() const {
//    return m_size;
//}

EventQueue::Timestamp EventQueue::addTaskAfter(Interval delay
                                          , std::function<void()> task) {
    Timestamp t = m_queueCurrentTime + delay;
    auto* event = &m_head;
    for(; event->m_next != nullptr; event = event->m_next) {
        if(event->m_eventTime <= t) {
            continue;
        }
        auto* e = new Event{event->m_next, t, task};
        event->m_next = e;
//        ++m_size;
        return t;
    }
    assert(event->m_next == nullptr);
    auto* e = new Event{nullptr, t, task};
    event->m_next = e;
//    ++m_size;
    return t;
}

void EventQueue::run() {
    while(m_head.m_next != nullptr) {
        auto* e = m_head.m_next;
        m_head.m_next = e->m_next;
        m_queueCurrentTime += e->m_eventTime;
        e->m_task();
//        --m_size;
        delete e;
    }
}

void EventQueue::stop() {
    while(m_head.m_next != nullptr) {
        auto* e = m_head.m_next;
        m_head.m_next = e->m_next;
        delete e;
//        --m_size;
    }
}

void EventQueue::setEndTime(Timestamp time) {
    addTaskAfter(time, [this] {
        stop();
    });
}


