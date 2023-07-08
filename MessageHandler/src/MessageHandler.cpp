#include <MessageHandler/MessageHandler.h>
#include <ctime>

// =============================================================================

MessageHandler::MessageHandler(
    const ErrorCallBack& errorCallback
):
    m_onErrorCallBack(errorCallback)
{}

MessageHandler::~MessageHandler()
{

}


void MessageHandler::handleMessage(const SensorMessage message)
{

}

void MessageHandler::setErrorCallBack(const ErrorCallBack& errorCallback)
{
    m_onErrorCallBack = errorCallback;
}








