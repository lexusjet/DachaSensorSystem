#pragma once

#include <SensorMessage/SensorMessage.h>
#include <DatabaseConnector/DatabaseConnector.h>
#include <memory>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>

class Database
{
    using InsertedCallback = std::function<void (const SensorMessage)>;
    using ErrorCallback = std::function<void (const SensorMessage, const std::exception)>;
    using QueryFromSensorMessage = std::function<std::string (const SensorMessage&)>;
private:
    std::string m_addres;
    std::string m_port;
    std::string m_name;
    std::string m_password;
    std::string m_databaseName;
    
    std::mutex m_fieldMutex;

    boost::asio::io_context m_ioContext;
    
    InsertedCallback onInsertedCallback;
    ErrorCallback onErrorCallback;

public:
    Database (
        const std::string& addres,
        const std::string& port,
        const std::string& name,
        const std::string& password,
        const std::string& databaseName,
        const InsertedCallback& insertCallback,
        const ErrorCallback& errorCallback
    );
    Database() = delete;
    ~Database();

public:
    void setDatabaseName (const std::string& name);
    void setErrorCallback(const ErrorCallback& callback);
    void setInsertedCallback(const InsertedCallback& callback);
    void insert (const SensorMessage& , const QueryFromSensorMessage&);

private:
    void insertFunction (const std::string databaseName, const SensorMessage message, const QueryFromSensorMessage);
};