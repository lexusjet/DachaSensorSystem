#include <ConfigLoader/ConfigLoader.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <mutex>

//PImpl -----------------------------------------------------------
struct ConfigLoader::PImpl
{
    using json = nlohmann::json;
private:
    std::string m_configName;
    std::mutex m_jsonMutex;
public:
    PImpl(const std::string& fileName):
        m_configName(fileName)
    {}
    ~PImpl()
    {
        std::lock_guard lock(m_jsonMutex);
    }

    std::string getDatabaseIp()
    {
        std::lock_guard lock(m_jsonMutex);
        std::fstream jsonFile(m_configName, std::fstream::out);
        if(!jsonFile.is_open())
            return "";
        try
        {
            json json;
            jsonFile >> json;
            return json["database"]["ip"].get<std::string>();
        }
        catch(const std::exception& exception)
        {
            return "";
        }
        
    }

    std::string getDatabasePort()
    {
        std::lock_guard lock(m_jsonMutex);
        std::fstream jsonFile(m_configName, std::fstream::out);
        if(!jsonFile.is_open())
            return "";
        try
        {
            json json;
            jsonFile >> json;
            return json["database"]["port"].get<std::string>();
        }
        catch(const std::exception& exception)
        {
            return "";
        }
        
    }

    std::string getDatabaseUserName()
    {
        std::lock_guard lock(m_jsonMutex);
        std::fstream jsonFile(m_configName, std::fstream::out);
        if(!jsonFile.is_open())
            return "";
        try
        {
            json json;
            jsonFile >> json;
            return json["database"]["name"].get<std::string>();
        }
        catch(const std::exception& exception)
        {
            return "";
        }
        
    }

    std::string getDatabasePassword()
    {
        std::lock_guard lock(m_jsonMutex);
        std::fstream jsonFile(m_configName, std::fstream::out);
        if(!jsonFile.is_open())
            return "";
        try
        {
            json json;
            jsonFile >> json;
            return json["database"]["password"].get<std::string>();
        }
        catch(const std::exception& exception)
        {
            return "";
        }
        
    }

    std::string getDatabaseName()
    {
        std::lock_guard lock(m_jsonMutex);
        std::fstream jsonFile(m_configName, std::fstream::out);
        if(!jsonFile.is_open())
            return "";
        try
        {
            json json;
            jsonFile >> json;
            return json["database"]["dataBaseName"].get<std::string>();
        }
        catch(const std::exception& exception)
        {
            return "";
        }
        
    }


    std::string getServerIp()
    {
        std::lock_guard lock(m_jsonMutex);
        std::fstream jsonFile(m_configName, std::fstream::out);
        if(!jsonFile.is_open())
            return "";
        try
        {
            json json;
            jsonFile >> json;
            return json["server"]["ip"].get<std::string>();
        }
        catch(const std::exception& exception)
        {
            return "";
        }
        
    }

    std::string getServerPort()
    {
        std::lock_guard lock(m_jsonMutex);
        std::fstream jsonFile(m_configName, std::fstream::out);
        if(!jsonFile.is_open())
            return "";
        try
        {
            json json;
            jsonFile >> json;
            return json["server"]["port"].get<std::string>();
        }
        catch(const std::exception& exception)
        {
            return "";
        }
        
    }


    std::string getLogFileName()
    {
        std::lock_guard lock(m_jsonMutex);
        std::fstream jsonFile(m_configName, std::fstream::out);
        if(!jsonFile.is_open())
            return "";
        try
        {
            json json;
            jsonFile >> json;
            return json["log"]["fileName"].get<std::string>();
        }
        catch(const std::exception& exception)
        {
            return "";
        }
        
    }

};
//~PImpl -----------------------------------------------------------




//ConfigLoader -----------------------------------------------------------
ConfigLoader::ConfigLoader(const std::string & fileName):
    m_pimpl(new PImpl(fileName))
{}

ConfigLoader::~ConfigLoader()
{}


std::string ConfigLoader::getDatabaseIp()
{
    return m_pimpl->getDatabaseIp();
}

std::string ConfigLoader::getDatabasePort()
{
    return m_pimpl->getDatabasePort();
}

std::string ConfigLoader::getDatabaseUserName()
{
    return m_pimpl->getDatabaseUserName();
}

std::string ConfigLoader::getDatabasePassword()
{
    return m_pimpl->getDatabasePassword();
}

std::string ConfigLoader::getDatabaseName()
{
    return m_pimpl->getDatabaseName();
}

std::string ConfigLoader::getServerIp()
{
    return m_pimpl->getServerIp();
}

std::string ConfigLoader::getServerPort()
{
    return m_pimpl->getServerPort();
}

std::string ConfigLoader::getLogFileName()
{
    return m_pimpl->getLogFileName();
}
//~ConfigLoader -----------------------------------------------------------