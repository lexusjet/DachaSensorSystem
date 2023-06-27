#pragma once

#include <string>
#include <memory>


class DatabaseConnector
{
private:
    class PImpl;
    std::unique_ptr<PImpl> m_pimpl;
public:
    DatabaseConnector(
        const std::string& addres,
        const std::string& port,
        const std::string& name,
        const std::string& password);
    DatabaseConnector() = delete;
    ~DatabaseConnector();

    void setName (const std::string&);
    void setPassword (const std::string&);
    void setAddres (const std::string&);
    void setPort (const std::string&);

    void execute(const std::string & queryStr);
    void connect();
    void disconnect();
    // std::vector<std::string> executeQuery(const std::string& queryStr);
};

class DataBaseException: public std::exception
{
public:
    enum DatabaseError
    {
        ConnectionError = 2023,
        LogInError = 1045,
        NoSchemaError = 1049,
        NoTableError = 1146,
        SyntaxError = 1064
    };

private:
    std::string m_msg;
    DatabaseError m_errorCode;

public:
    DataBaseException (const std::string& msg, DatabaseError error): 
        m_msg(msg), m_errorCode(error) {}
    std::string what () {return m_msg;}
    DatabaseError getErrorCode () {return m_errorCode;}
};