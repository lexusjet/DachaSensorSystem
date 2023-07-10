#pragma once

#include <string>
#include <memory>

// =============================================================================
class ConfigLoader
{
private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pimpl;

public:
    ConfigLoader(const std::string &);
    ~ConfigLoader();
    std::string getDatabaseIp();
    std::string getDatabasePort();
    std::string getDatabaseUserName();
    std::string getDatabasePassword();
    std::string getDatabaseName();

    std::string getServerIp();
    std::string getServerPort();

    std::string getLogFileName();

};