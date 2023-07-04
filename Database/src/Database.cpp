#include <Database/Database.h>
#include <boost/bind.hpp>
Database::Database(
        const std::string& addres,
        const std::string& port,
        const std::string& name,
        const std::string& password,
        const std::string& databaseName
    ):  m_addres(addres),
        m_port(port),
        m_name(name),
        m_password(password),
        m_databaseName(databaseName)
{}

Database::~Database()
{
    m_ioContext.run();
}

void Database::insert(const SensorMessage& message)
{
    m_ioContext.post(boost::bind(&Database::insert, this, message));
    std::thread workerThread(
        [](boost::asio::io_context& io){io.run();},
        std::ref(m_ioContext)
    );
    workerThread.detach();
}

void Database::insertFunction(const SensorMessage message)
{
    DatabaseConnector connector(m_addres, m_port, m_name, m_password);
    try
    {
        connector.connect();
        connector.execute("USE " + m_databaseName);
        std::string query = "INSERT INTO ";
        
        //TODO: convert sensor message value to str
        
        
        connector.execute(query);
    }
    catch(DataBaseException& exception)
    {
        onErrorCallback(message, exception);
        return;
    }
    onInsetedCallback(message);
}

