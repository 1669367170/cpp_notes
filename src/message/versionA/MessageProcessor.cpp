#include "MessageProcessor.h"

void MessageProcessor::InitMessageMap() {
    Clear();
    RegisterMessageFunc(MessageType::MESSAGE_START, &MessageProcessor::OnStart);
    RegisterMessageFunc(MessageType::MESSAGE_PROCESSING, &MessageProcessor::OnProcessing);
    RegisterMessageFunc(MessageType::MESSAGE_DONE, &MessageProcessor::OnDone);
    RegisterMessageFunc(MessageType::MESSAGE_EXCEPTION, &MessageProcessor::OnException);
}

bool MessageProcessor::OnStart(Message &message) {
    std::cout << "Work Start" << std::endl;
    return true;
}

bool MessageProcessor::OnProcessing(Message &message) {
    std::cout << "Work Processing..." << message.Info().c_str() << "%" << std::endl;
    return true;
}

bool MessageProcessor::OnDone(Message &message) {
    std::cout << "Work Done" << std::endl;
    return false; // 返回false，MessageProcessor停止消息接收，线程退出
}

bool MessageProcessor::OnException(Message &message) {
    std::cout << "Work Exception!" << std::endl;
    return true;
}
