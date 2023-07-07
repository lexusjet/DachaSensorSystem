#include <MessageHandler/MessageHandler.h>
#include <ctime>

// =============================================================================

MessageHandler::MessageHandler(
    const SensorMessageValidator& valid,
    const SensorMessageConverter& convert,
    const ErrorCallBack& errorCallback
):
    m_validator(valid),
    m_converter(convert),
    m_onErrorCallBack(errorCallback)
{}

MessageHandler::~MessageHandler()
{

}


void MessageHandler::handleMessage(const SensorMessage& message)
{
    if(!validateSensorMessage(message))
    {
        //error log
    }
    
    std::string query = convertSensorMessageToQuery(message);

    if(query.size() == 0)
    {
        // error log
    }
    
}

void MessageHandler::setErrorCallBack(const ErrorCallBack& errorCallback)
{
    m_onErrorCallBack = errorCallback;
}

bool MessageHandler::validateSensorMessage(const SensorMessage& message)
{
    return m_validator.validate(message);
}

std::string MessageHandler::convertSensorMessageToQuery(
    const SensorMessage& message
)
{
    
    std::string table_name = "Temperature_in_locations";
    std::string colomnNames = R"(
        date, 
        time, 
        temperature_сelsius, 
        locationId, 
        placeId 
        )";
    // date time data location numberInLocation


    time_t seconds = time(NULL);
    tm* timeInfo = localtime(&seconds);

    std::string date(
        std::to_string(timeInfo->tm_mday) +
        "-" +
        std::to_string(timeInfo->tm_mon) +
        "-" +
        std::to_string(timeInfo->tm_year + 1900)
    );

    std::string time(
        std::to_string(timeInfo->tm_hour) +
        ":" +
        std::to_string(timeInfo->tm_min)
    );

    std::string dataValue = std::to_string(
        static_cast<int>(message.getData())
    );

    std::string locationValue = std::to_string(
        static_cast<size_t>(message.getLocation())
    );
    std::string numberInLocationValue = std::to_string(
        static_cast<size_t>(message.getNumberInLocation())
    );
    std::string values( 
        "VALUES ( " +
        date + " " +
        time + " " +
        dataValue + " " +
        locationValue + " " +
        numberInLocationValue + " " +
        " )"
    );

    std::string anser(
        "INSERT INTO " +
        table_name + " " +
        colomnNames + " " +
        values
    );

    return anser;
}
void MessageHandler::setValidator(const SensorMessageValidator& b)
{
    m_validator = b;
}

void MessageHandler::setConverter(const SensorMessageConverter& b)
{
    m_converter = b;
}

// version
// location
// numberInLocation
// dataType
// extension
// reserve
