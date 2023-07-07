#pragma once

#include <functional>
#include <mutex>
#include <SensorMessage/SensorMessage.h>
#include <SensorMessageConverter/SensorMessageConverter.h>
#include <SensorMessageValidator/SensorMessageValidator.h>

// =============================================================================

class MessageHandler
{
    using ErrorCallBack = std::function<void (SensorMessage, std::string)>;
    using EndCallBack = std::function<void (const SensorMessage&)>;
    // возможно нужен колбек на действие если все прошло хорошо

private:
    SensorMessageValidator m_validator;
    SensorMessageConverter m_converter;
    ErrorCallBack m_onErrorCallBack;
    
public:
    MessageHandler() = delete;
    MessageHandler(
        const SensorMessageValidator&,
        const SensorMessageConverter&,
        const ErrorCallBack&
    );
    ~MessageHandler();

public:
    void handleMessage(SensorMessage);
    void setValidator(const SensorMessageValidator&);
    void setConverter(const SensorMessageConverter&);
    void setErrorCallBack(const ErrorCallBack&);
private:
    std::string convertSensorMessageToQuery(const SensorMessage&);
    bool validateSensorMessage(const SensorMessage&);
        

};