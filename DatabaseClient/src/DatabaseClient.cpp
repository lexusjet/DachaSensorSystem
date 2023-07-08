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

void DatabaseClient::setDatabaseName(const std::string& name)
{
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    m_databaseName = name;
}

void DatabaseClient::execute(const std::string& query)
{   
    std::cout << "execute start" << std::endl;
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    m_ioService.post(
        boost::bind(
            &DatabaseClient::executeFunction,
            this,
            m_databaseName,
            query
        )
    );
    std::cout << "execute end" << std::endl;
}
std::string DatabaseClient::getQueryFromSensorMessage(const SensorMessage&)
{
    return "";
}

void DatabaseClient::executeFunction(
    const std::string databaseName,
    const std::string query
)
{
    std::cout << "executeFunction start" << std::endl;
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
    std::cout << "executeFunction end" << std::endl;
}

