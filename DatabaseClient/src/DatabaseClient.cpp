#include <DatabaseClient/DatabaseClient.h>
#include <boost/bind/bind.hpp>


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
        onErrorCallback(errorCallback)
{}

DatabaseClient::~DatabaseClient()
{
    m_ioContext.run();
}

void DatabaseClient::setDatabaseName(const std::string& name)
{
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    m_ioContext.run();
    m_databaseName = name;
}

void DatabaseClient::execute(const std::string& query)
{   
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    
    m_ioContext.post(boost::bind(
            &DatabaseClient::executeFunction,
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

void DatabaseClient::setErrorCallback(const ErrorCallback& callback)
{
    m_ioContext.run();
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    onErrorCallback = callback;
}

void DatabaseClient::setInsertedCallback(const InsertedCallback& callback)
{
    m_ioContext.run();
    std::lock_guard<std::mutex> lock(m_fieldMutex);
    onInsertedCallback = callback;
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
    catch(const std::exception& exception)
    {
        onErrorCallback(query, exception);
        return;
    }
    onInsertedCallback(query);
}

