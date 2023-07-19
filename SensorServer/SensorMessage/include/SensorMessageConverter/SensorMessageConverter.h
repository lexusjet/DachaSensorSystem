#pragma once
#include <unordered_map>
#include <string>
#include <SensorMessage/SensorMessage.h>

// =============================================================================

class SensorMessageConverter
{
private:
    std::unordered_map<size_t, std::string> m_locationsMap;
    std::unordered_map<size_t, std::string> m_numberInLocationsMap;
    std::unordered_map<size_t, std::string> m_dataTypes;

public:
    SensorMessageConverter(
        const std::unordered_map<size_t, std::string>&,
        const std::unordered_map<size_t, std::string>&,
        const std::unordered_map<size_t, std::string>&
    );

    SensorMessageConverter() = delete;
    ~SensorMessageConverter() = default;

public:
    void setLocation(const std::unordered_map<size_t, std::string>& );
    void setNumberInLocation(const std::unordered_map<size_t, std::string>& );
    void setDataType(const std::unordered_map<size_t, std::string>& );

public:
    std::string convertLocation(const SensorMessage& );
    std::string convertNumberInLocation(const SensorMessage& );
    std::string convertDataType(const SensorMessage& );

};