#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <stack>
#include <atomic>
#include <functional>
#include <condition_variable>

#include <SensorMessage/SensorMessage.h>
// =============================================================================

class MessageHandler
{
    using ErrorCallBack = std::function<void (const std::string&)>;
    // возможно нужен колбек на действие если все прошло хорошо

    public:
        MessageHandler();
        ~MessageHandler();
        void start();
        void stop();
    public:
        void addMessage(const SensorMessage& );
    
    private:
        void hendlerLoop();
        bool validateMessage(const SensorMessage&);
    private:
        std::thread m_handlerThread;
        std::mutex m_sensorMessageStackMutex;
        std::condition_variable m_condition;
        std::atomic<bool> m_isStop;
        std::stack<SensorMessage> m_sensorMessageStack;
        ErrorCallBack m_onErrorCallBack;
};