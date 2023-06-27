#include <DatabaseInterface/DatabaseInterface.h>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>


class DatabaseConnector::PImpl
{
private:
    std::string m_addres;
    std::string m_port;
    std::string m_name;
    std::string m_password;
    std::unique_ptr<sql::Connection> m_connection;
    

public:
    PImpl () = default;
    PImpl (
        const std::string& addres,
        const std::string& port,
        const std::string& name,
        const std::string& password):
            m_addres(addres),
            m_port(port),
            m_name(name),
            m_password(password)
        {}
    
    void connect()
    {
        if (!m_connection)
        {}
        try
        {
            std::string connectionAddres = "tcp://" + m_addres + ":" + m_port;
            sql::Driver * driver = get_driver_instance();
            m_connection.reset(driver->connect(connectionAddres, m_name, m_password));
        }
        catch(sql::SQLException & exception)
        {
            switch (exception.getErrorCode())
            {
                // can`t connect to data base
                case 2003: throw DataBaseException("Can`t connect to database", DataBaseException::ConnectionError);
                    break;
                // incorect user name or password
                case 1045: throw DataBaseException("Incorect password or username", DataBaseException::LogInError);
                    break;
            }
        }
    }
    void execute (const std::string& queryStr)
    {
        try
        {
            std::unique_ptr<sql::Statement> statement(m_connection->createStatement());
            statement->execute(queryStr);
        }
        catch (sql::SQLException& exception)
        {
            switch (exception.getErrorCode())
            {
                // can`t connect to data base
                case 2003: throw DataBaseException("Can`t connect to database", DataBaseException::ConnectionError);
                    break;
                // no such Schema
                case 1049: throw DataBaseException("No such Schema", DataBaseException::NoSchemaError);
                    break;
                // no such table
                case 1146: throw DataBaseException("No such table", DataBaseException::NoTableError);
                    break;
                // incorect sytax of query
                case 1064:  throw DataBaseException("Sytax error", DataBaseException::SyntaxError);
                    break;
            }
        }
    }

    void disconnect ()
    {
        if (!m_connection->isClosed())
        {
            m_connection->close();
        }
    }

    void setName (const std::string& name) { m_name = name;};
    void setPassword (const std::string& pass) {m_password = pass;};
    void setAddres (const std::string& addres) {m_addres = addres;};
    void setPort (const std::string& port) {m_port = port;};
    
    // std::vector<std::string> executeQuery(const std::string& queryStr){}


};

//------------------------------------------------------

DatabaseConnector::DatabaseConnector(
        const std::string& addres,
        const std::string& port,
        const std::string& name,
        const std::string& password): 
        m_pimpl(new PImpl(addres, port, name, password))
{}

DatabaseConnector::~DatabaseConnector ()
{}

void DatabaseConnector::setName(const std::string& name)
{
    m_pimpl->setName(name);
}

void DatabaseConnector::setPassword(const std::string& str)
{
    m_pimpl->setPassword(str);
}

void DatabaseConnector::setAddres(const std::string& str) 
{
    m_pimpl->setAddres(str);
}

void DatabaseConnector::execute(const std::string& query)
{
    m_pimpl->execute(query);
}

void DatabaseConnector::connect ()
{
    m_pimpl->connect();
}

void DatabaseConnector::disconnect ()
{
    m_pimpl->disconnect();
}
// std::vector<std::string> DatabaseInterface::executeQuery(const std::string& query)
// {
    // return m_pimpl->executeQuery(query);
// }