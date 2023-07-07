#include <Database/Database.h>
#include <boost/bind/bind.hpp>


Database::Database(
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
        onErrorCallback(errorCallback)
{}

Database::~Database()
{
    m_ioContext.run();
}

void Database::setDatabaseName(const std::string& name)
{
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    m_ioContext.run();
    m_databaseName = name;
}

void Database::execute(const std::string& query)
{   
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    
    m_ioContext.post(boost::bind(
            &Database::executeFunction,
            this,
            m_databaseName,
            query
        )
    );

    std::thread workerThread(
        [](boost::asio::io_context& io){io.run();},
        std::ref(m_ioContext)
    );
    workerThread.detach();
}

void Database::setErrorCallback(const ErrorCallback& callback)
{
    m_ioContext.run();
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    onErrorCallback = callback;
}

void Database::setInsertedCallback(const InsertedCallback& callback)
{
    m_ioContext.run();
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    onInsertedCallback = callback;
}

void Database::executeFunction(
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
    catch(const std::exception& exception)
    {
        onErrorCallback(message, exception);
        return;
    }
    onInsertedCallback(message);
}

