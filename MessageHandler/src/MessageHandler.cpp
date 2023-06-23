#include <MessageHandler/MessageHandler.h>

MessageHandler::MessageHandler()
    : m_isStop(true)
{}

MessageHandler::~MessageHandler()
{
    m_isStop = true;
    if(m_handlerThread.joinable())
        m_handlerThread.join();

}

void MessageHandler::start()
{
    if(m_handlerThread.joinable())
        m_handlerThread.join();
    m_isStop = false;
    m_handlerThread = std::thread(&MessageHandler::hendlerLoop, this);
}

void MessageHandler::stop()
{
    m_isStop = true;
}

void MessageHandler::addMessage(const SensorMessage& message)
{
    if(m_isStop)
    {   
        m_onErrorCallBack(std::string("can`t add SensorMessage MessageHandler is stoped"));
        return;
    }
    std::lock_guard lock(m_sensorMessageStackMutex);
    m_sensorMessageStack.push(message);
    m_condition.notify_all();
}

void MessageHandler::hendlerLoop()
{
    while(!m_isStop)
    {

        std::unique_lock lock(m_sensorMessageStackMutex);
        m_condition.wait(lock, [&](){return m_sensorMessageStack.size();});
        SensorMessage message = m_sensorMessageStack.top();
        m_sensorMessageStack.pop();
        lock.unlock();
        
        if(validateMessage(message))
        {
            //TODO action if message is valid
            // для начала наверное парсинг 
            // потом отправка в бд
        }
        else
        {
            m_onErrorCallBack("SensorMessage isn`t valid");
        }

    }
}

bool MessageHandler::validateMessage(const SensorMessage& message)
{
    // TODO: возможно стоит сделать файл настройки валидации
    // типо txt файл который будет при старте загружаться 
    // в ктором будут обознчены занчение 
    // для того или иного параметра сообщения



    return true;
}