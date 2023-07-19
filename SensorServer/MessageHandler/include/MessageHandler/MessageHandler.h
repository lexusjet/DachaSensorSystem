#pragma once

#include <functional>
#include <SensorMessage/SensorMessage.h>

// =============================================================================

class MessageHandler
{
    using ErrorCallBack = std::function<void (SensorMessage, std::string)>;
    using EndCallBack = std::function<void (const SensorMessage&)>;
    // возможно нужен колбек на действие если все прошло хорошо

private:
    ErrorCallBack m_onErrorCallBack;
    
public:
    MessageHandler() = delete;
    MessageHandler(
        const ErrorCallBack&
    );
    ~MessageHandler();

public:
    void handleMessage(const SensorMessage);
    void setErrorCallBack(const ErrorCallBack&);
private:
    std::string convertSensorMessageToQuery(const SensorMessage&);
    bool validateSensorMessage(const SensorMessage&);
        

};