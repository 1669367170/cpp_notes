#include "MessageBus.h"

std::mutex                  MessageBus::s_mt;
std::condition_variable     MessageBus::s_cv;
std::list<Message *>        MessageBus::s_queue;

void MessageBus::Clear() {
    std::list<Message *>::iterator it = s_queue.begin();
    for (; it != s_queue.end(); ++it) {
        delete *it;
    }
    s_queue.clear();
}

void MessageBus::Push(Message *pMessage) {
    std::unique_lock<std::mutex> lock(s_mt);
    s_queue.push_back(pMessage);
    s_cv.notify_all();
}

void MessageBus::Push(MessageType type) {
    Push(type, std::string(""), MessagePriority::MP_NORMAL);
}

void MessageBus::Push(MessageType type, std::string info) {
    Push(type, info, MessagePriority::MP_NORMAL);
}

void MessageBus::Push(MessageType type, std::string info, MessagePriority priority) {
    Message *pMessage = new Message(type, info, priority);
    Push(pMessage);
}

MessagePtr MessageBus::Pop() {
    Message *pMessage = nullptr;
    {
        std::unique_lock<std::mutex> lock(s_mt);
        while (s_queue.empty()) {
            s_cv.wait(lock);
        }
        pMessage = s_queue.front();
        s_queue.pop_front();
    }
    return MessagePtr(pMessage);
}
