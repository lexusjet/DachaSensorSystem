#pragma once

#include <SensorMessage/SensorMessage.h>
#include <DatabaseConnector/DatabaseConnector.h>
#include <ServerListener/ServerListener.h>
#include <memory>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>

class DatabaseClient : public ServerListener
{
    using InsertedCallback = std::function<void (const std::string)>;
    using ErrorCallback = std::function<void (const std::string, DataBaseException)>;
private:
    std::string m_addres;
    std::string m_port;
    std::string m_name;
    std::string m_password;
    std::string m_databaseName;
    
    InsertedCallback onInsertedCallback;
    ErrorCallback onErrorCallback;

    std::mutex m_fieldMutex;
    boost::asio::io_service m_ioService;
    std::shared_ptr<boost::asio::io_service::work> m_workLock;
    std::thread m_thread;


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
    virtual ~DatabaseClient();

    virtual void notify(const SensorMessage&) override;

public:
    void setDatabaseName(const std::string& name);
    void execute(const std::string&);

private:
    std::string getQueryFromSensorMessage(const SensorMessage&);
    void executeFunction(
        const std::string databaseName,
        const std::string query
    );
};
