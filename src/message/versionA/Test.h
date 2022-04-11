// 原文链接：https://www.jianshu.com/p/f269fa100874 （用C++实现数据总线的方法系列(下)：消息总线和消息处理）

#include "MessageProcessor.h"
#include <thread>

namespace MessageBusVersionA {
    void WaitOneSencond() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void SendMessage() {
        WaitOneSencond();
        MessageBus::Push(MessageType::MESSAGE_START);
        int i = 1;
        while (i <= 10) {
            WaitOneSencond();
            MessageBus::Push(MessageType::MESSAGE_PROCESSING, std::to_string(i * 10));
            ++i;
        }
        WaitOneSencond();
        MessageBus::Push(MessageType::MESSAGE_DONE);
    }

    // 调用执行
    void Test() {
        MessageProcessor msgProc;
        std::thread thProc([&] { msgProc.ON(); }); // 消息接收处理线程
        std::thread thSend(SendMessage); // 消息发送线程

        thProc.join();
        thSend.join();
    }
}
