#pragma once

#include "MessageBus.h"
#include <iostream>
#include <atomic>
#include <unordered_map>

// 消息处理基类
// (ON函数)处理流程：注册好消息响应函数 -> 从MessageBus取消息 -> 调用InvokeHanlderFunc -> 查找消息响应函数 -> 处理
template<typename T>
class MessageHandler {
public:
    MessageHandler() : m_running(false) {};

    ~MessageHandler() { OFF(); };

    // 取消消息接收
    void OFF() {
        std::cout << "Message Handling OFF!" << std::endl;
        m_running = false;
    }

    // 开启消息接收
    void ON() {
        std::cout << "Message Handling ON!" << std::endl;
        m_running = true;
        // 调用继承类的InitMessageMap将消息响应函数注册好
        static_cast<T *>(this)->InitMessageMap();
        while (m_running) {
            // 在MessageBus里取消息，调用InvokeHandlerFunc函数，基于类型查找到相应消息响应函数；
            // 然后调用该函数将消息作为入参传递该函数进行处理；
            MessagePtr pMessage = MessageBus::Pop();
            m_running = InvokeHandlerFunc(*pMessage);
        }
    }

protected:
    using MessageHandlerFunc = bool (T::*)(Message &);
    // key: 消息类型，value：消息响应函数；关联关系通过RegisterMessageFunc函数(注册消息响应函数)完成
    using MessageMap = std::unordered_map<MessageType, MessageHandlerFunc>;

    MessageMap m_messageMap;

    void RegisterMessageFunc(MessageType type, MessageHandlerFunc func) {
        m_messageMap[type] = func;
    }

    bool InvokeHandlerFunc(Message &message) {
        auto it = m_messageMap.find(message.Type());
        if (it != m_messageMap.end()) {
            T *pThis = static_cast<T *>(this);
            return (pThis->*(it->second))(message);
        } else {
            std::cout << "Message Type: " << message.Type() << " not handler function!" << std::endl;
            return false;
        }
    }

    void Clear() {
        m_messageMap.clear();
    }

private:
    std::atomic_bool m_running;
};
