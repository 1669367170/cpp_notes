#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <list>

// 消息类型
enum MessageType {
    MESSAGE_NONE = 0,
    MESSAGE_START,
    MESSAGE_PROCESSING,
    MESSAGE_DONE,
    MESSAGE_EXCEPTION
};

// 消息优先级
enum MessagePriority {
    MP_LOW = 0,
    MP_NORMAL,
    EP_HIGH
};

// 消息的结构体
class Message {
public:
    Message() : m_type(MESSAGE_DONE), m_priority(MP_NORMAL) {}

    Message(MessageType type, std::string info, MessagePriority priority) :
            m_type(type), m_info(info), m_priority(priority) {}

    ~Message() = default;

    MessageType Type() { return m_type; }

    MessagePriority Priority() { return m_priority; }

    std::string Info() { return m_info; }

private:
    MessageType m_type;
    MessagePriority m_priority;
    std::string m_info; // 消息数据
};

using MessagePtr = std::shared_ptr<Message>;

// 消息总线实现：1) 有存放消息的消息链表；2) Push函数New出一个Message对象，存放需要发送的消息数据，放入到消息链表中；
// 3) Pop函数得到这个对象的指针，并作为初始化参数传递给一个MessagePtr智能指针，外部调用处理完消息后，消息对象被释放；
class MessageBus {
public:
    static void Clear();

    static void Push(Message *pMessage);

    static void Push(MessageType type);

    static void Push(MessageType type, std::string info);

    static void Push(MessageType type, std::string info, MessagePriority priority);

    static MessagePtr Pop();

private:
    static std::mutex s_mt; // 构成锁的互斥量
    static std::condition_variable s_cv; // 用于多线程同步的条件变量
    static std::list<Message *> s_queue; // 存放消息的消息链表
};
