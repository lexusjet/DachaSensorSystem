#pragma once

#include <SensorMessage/SensorMessage.h>
#include <DatabaseConnector/DatabaseConnector.h>
#include <memory>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>

class DatabaseClient
{
    using InsertedCallback = std::function<void (const std::string)>;
    using ErrorCallback = std::function<void (const std::string, const std::exception)>;
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
    DatabaseClient (
        const std::string& addres,
        const std::string& port,
        const std::string& name,
        const std::string& password,
        const std::string& databaseName,
        const InsertedCallback& insertCallback,
        const ErrorCallback& errorCallback
    );
    DatabaseClient() = delete;
    ~DatabaseClient();

public:
    void setDatabaseName(const std::string& name);
    void setErrorCallback(const ErrorCallback& callback);
    void setInsertedCallback(const InsertedCallback& callback);
    void execute(const std::string&);

private:
    void executeFunction(
        const std::string databaseName,
        const std::string query
    );
};