#pragma once

#include <SensorMessage/SensorMessage.h>
#include <DatabaseConnector/DatabaseConnector.h>
#include <memory>
#include <boost/asio.hpp>
#include <thread>

class Database
{
private:
    std::string m_addres;
    std::string m_port;
    std::string m_name;
    std::string m_password;
    std::string m_databaseName;
    
    boost::asio::io_context m_ioContext;

    std::function<void (SensorMessage)> onInsetedCallback;
    std::function<void (SensorMessage, DataBaseException)> onErrorCallback;
    

public:
    Database (
        const std::string& addres,
        const std::string& port,
        const std::string& name,
        const std::string& password,
        const std::string& databaseName
    );
    ~Database();
    Database() = delete;

    void setDatabaseName ();
    void insert (const SensorMessage&);
private:
    void insertFunction (const SensorMessage message);
};