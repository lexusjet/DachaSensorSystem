#include <DatabaseClient/DatabaseClient.h>
#include <boost/bind/bind.hpp>
#include <boost/make_shared.hpp>
#include <memory>
#include <iostream>

DatabaseClient::DatabaseClient(
        const std::string& addres,
        const std::string& port,
        const std::string& name,
        const std::string& password,
        const std::string& databaseName,
        const InsertedCallback& insertCallback,
        const ErrorCallback& errorCallback
    ):  m_addres(addres),
        m_port(port),
        m_name(name),
        m_password(password),
        m_databaseName(databaseName),
        onInsertedCallback(insertCallback),
        onErrorCallback(errorCallback),
        m_workLock(std::make_shared<boost::asio::io_service::work>(m_ioService)),
        m_thread(boost::bind(&boost::asio::io_service::run, &m_ioService))
{}

DatabaseClient::~DatabaseClient()
{
    m_workLock.reset();
    m_thread.join();
}

void DatabaseClient::notify(const SensorMessage& message)
{
    execute(getQueryFromSensorMessage(message));
}

void DatabaseClient::setDatabaseName(const std::string& name)
{
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    m_databaseName = name;
}

void DatabaseClient::execute(const std::string& query)
{   
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    m_ioService.post(
        boost::bind(
            &DatabaseClient::executeFunction,
            this,
            m_databaseName,
            query
        )
    );
}
std::string DatabaseClient::getQueryFromSensorMessage(const SensorMessage& message)
{

    // мне нужно получить тут имя нужной табицы и имена полей
    std::string table_name = "Temperature_in_locations";
    std::string colomnNames = R"(
        date, 
        time, 
        temperature_сelsius, 
        locationId, 
        placeId 
        )";
    //date time data location numberInLocation


    time_t seconds = time(NULL);
    tm* timeInfo = localtime(&seconds);
    
    std::string date(
        std::to_string(timeInfo->tm_year + 1900) +
        "-" +
        std::to_string(timeInfo->tm_mon) +
        "-" +
        std::to_string(timeInfo->tm_mday)
    );

    std::string time(
        std::to_string(timeInfo->tm_hour) +
        ":" +
        std::to_string(timeInfo->tm_min) +
        ":" +
        std::to_string(timeInfo->tm_sec)
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

    //   "INSERT INTO table (colomn_name1, ... , colomn_nameN) VALUES( colomn_value, ... , colomn_valueN)"
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

void DatabaseClient::executeFunction(
    const std::string databaseName,
    const std::string query
)
{
    DatabaseConnector connector(m_addres, m_port, m_name, m_password);
    try
    {
        connector.connect();
        connector.execute("USE " + databaseName);        
        connector.execute(query);
    }
    catch(const DataBaseException& exception)
    {
        onErrorCallback(query, exception);
        return;
    }
    onInsertedCallback(query);
}

