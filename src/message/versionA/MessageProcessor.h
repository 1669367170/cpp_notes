#pragma once

#include "MessageHandler.h"

// 消息处理业务类-实际的消息处理类
// 实现了一些消息响应函数，并将消息响应函数注册到基类的messageMap中
class MessageProcessor : public MessageHandler<MessageProcessor> {
public:
    MessageProcessor() = default;

    ~MessageProcessor() = default;

    // 调用基类的RegisterMessageFunc，将消息响应函数注册到基类的messageMap中
    void InitMessageMap();

private:
    // 消息响应函数
    bool OnStart(Message &message);

    bool OnProcessing(Message &message);

    bool OnDone(Message &message);

    bool OnException(Message &message);
};
