#include <SensorMessageValidator/SensorMessageValidator.h>
#include <algorithm>

// =============================================================================

SensorMessageValidator::SensorMessageValidator(
    const std::vector<size_t>& a,
    const std::unordered_map<size_t, std::vector<size_t>>& b,
    const std::vector<size_t>& c,
    const std::vector<int>& d,
    const std::vector<int>& e
):
    m_verionsVec(a),
    m_locationRoomsMap(b),
    m_dataTypesVec(c),
    m_extenionsVec(d),  
    m_reservesVec(e)
{}

void SensorMessageValidator::setVersions(const std::vector<size_t>& a)
{
    m_verionsVec = a;
}

void SensorMessageValidator::setLocationNumberInLocation(const std::unordered_map<size_t, std::vector<size_t>>& a)
{
    m_locationRoomsMap = a;
}

void SensorMessageValidator::setDataTypesconst(const std::vector<size_t>& a)
{
    m_dataTypesVec = a;
}

void SensorMessageValidator::setExtenions(const std::vector<int>& a)
{
    m_extenionsVec = a;
}

void SensorMessageValidator::setReservs(const std::vector<int>& a)
{
    m_reservesVec = a;
}

bool SensorMessageValidator::validate(const SensorMessage& a)
{
    // validate version
    auto versionItr = std::find(m_verionsVec.begin(), m_verionsVec.end(), a.getVersion());
    if( versionItr == m_verionsVec.end())
        return false;

    // validate location and numberInLocation
    auto locationItr = m_locationRoomsMap.find(a.getLocation()); 
    if(locationItr == m_locationRoomsMap.end())
        return false;
    auto roomsItr = std::find(
        locationItr->second.begin(),
        locationItr->second.end(),
        a.getNumberInLocation()
    );
    if(roomsItr == locationItr->second.end())
        return false;

    // validate dataType
    auto dataTypeItr = std::find(
        m_dataTypesVec.begin(),
        m_dataTypesVec.end(),
        a.getDataType()
    );
    if(dataTypeItr == m_dataTypesVec.end())
        return false;

    // validate extenion
    auto extItr = std::find(
        m_extenionsVec.begin(),
        m_extenionsVec.end(),
        a.getExtension()
    );
    if( extItr == m_extenionsVec.end())
        return false;

    // validate reserve
    auto reserveItr = std::find(
        m_reservesVec.begin(),
        m_reservesVec.end(),
        a.getReserve()
    );
    if(reserveItr == m_reservesVec.end())
        return false;
    //~validate reserve

    return true;
}