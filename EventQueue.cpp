#include "EventQueue.h"
#include <ios>
#include "Utils.h"
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

EventQueue::Timestamp EventQueue::addTask(Duration deltaTime
                                          , std::function<void()> task) {
    Timestamp t = m_queueCurrentTime + deltaTime;
    auto* event = &m_head;
//    for(; event != nullptr; event = event->m_next) {
//        if(event->m_eventTime > t) {
            auto* e = new Event{event->m_next, t, task};
            event->m_next = e;
//        }
//    }
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


