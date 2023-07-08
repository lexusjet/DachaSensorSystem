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




    
    // std::string table_name = "Temperature_in_locations";
    // std::string colomnNames = R"(
    //     date, 
    //     time, 
    //     temperature_сelsius, 
    //     locationId, 
    //     placeId 
    //     )";
    // // date time data location numberInLocation


    // time_t seconds = time(NULL);
    // tm* timeInfo = localtime(&seconds);

    // std::string date(
    //     std::to_string(timeInfo->tm_mday) +
    //     "-" +
    //     std::to_string(timeInfo->tm_mon) +
    //     "-" +
    //     std::to_string(timeInfo->tm_year + 1900)
    // );

    // std::string time(
    //     std::to_string(timeInfo->tm_hour) +
    //     ":" +
    //     std::to_string(timeInfo->tm_min)
    // );

    // std::string dataValue = std::to_string(
    //     static_cast<int>(message.getData())
    // );

    // std::string locationValue = std::to_string(
    //     static_cast<size_t>(message.getLocation())
    // );
    // std::string numberInLocationValue = std::to_string(
    //     static_cast<size_t>(message.getNumberInLocation())
    // );
    // std::string values( 
    //     "VALUES ( " +
    //     date + " " +
    //     time + " " +
    //     dataValue + " " +
    //     locationValue + " " +
    //     numberInLocationValue + " " +
    //     " )"
    // );







