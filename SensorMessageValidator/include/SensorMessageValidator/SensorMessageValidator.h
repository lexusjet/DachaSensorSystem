#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <SensorMessage/SensorMessage.h>

// =============================================================================

class SensorMessageValidator
{
private:
    std::vector<size_t> m_verionsVec;
    std::unordered_map<size_t, std::vector<size_t>> m_locationRoomsMap;
    std::vector<size_t> m_dataTypesVec;
    std::vector<int> m_extenionsVec;
    std::vector<int> m_reservesVec;
    
public:
    SensorMessageValidator(
        const std::vector<size_t>& a,
        const std::unordered_map<size_t, std::vector<size_t>>& b,
        const std::vector<size_t>& c,
        const std::vector<int>& d,
        const std::vector<int>& e
    );
    SensorMessageValidator() = delete;
    ~SensorMessageValidator() = default;

public:
    void setVersions(const std::vector<size_t>& );
    void setLocationNumberInLocation(const std::unordered_map<size_t, std::vector<size_t>>& );
    void setDataTypesconst(const std::vector<size_t>& );
    void setExtenions(const std::vector<int>& );
    void setReservs(const std::vector<int>& );

    bool validate(const SensorMessage& );

};