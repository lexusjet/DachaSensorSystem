#include <SensorMessageConverter/SensorMessageConverter.h>

SensorMessageConverter::SensorMessageConverter(
    const std::unordered_map<size_t, std::string>& a,
    const std::unordered_map<size_t, std::string>& b,
    const std::unordered_map<size_t, std::string>& c
):
    m_locationsMap(a),
    m_numberInLocationsMap(b),
    m_dataTypes(c)
{}

void SensorMessageConverter::setLocation(const std::unordered_map<size_t, std::string>& a)
{
    m_locationsMap = a;
}

void SensorMessageConverter::setNumberInLocation(const std::unordered_map<size_t, std::string>& a)
{
    m_numberInLocationsMap = a;
}

void SensorMessageConverter::setDataType(const std::unordered_map<size_t, std::string>& a)
{
    m_dataTypes = a;
}

std::string SensorMessageConverter::convertLocation(const SensorMessage& a)
{
    auto locationItr = m_locationsMap.find(a.getLocation());
    if(locationItr == m_locationsMap.end())
        return ("");
    return locationItr->second;
}

std::string SensorMessageConverter::convertNumberInLocation(const SensorMessage& a)
{
    auto itr = m_numberInLocationsMap.find(a.getNumberInLocation());
    if(itr == m_numberInLocationsMap.end())
        return ("");
    return itr->second;
}


std::string SensorMessageConverter::convertDataType(const SensorMessage& a)
{
    auto itr = m_dataTypes.find(a.getDataType());
    if(itr == m_dataTypes.end())
        return ("");
    return itr->second;
}
