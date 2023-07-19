#pragma once

#include <string>


// нужно сохранить таблица данные датчика
// или можно сохронять сразу запросы 
// наверное проще сохронять запросы 



class DelayedQueryManager
{
private: 
    std::string m_buuferFileName;
    
public:
    DelayedQueryManager();
    ~DelayedQueryManager();

    void addQueryToQueue(const std::string& query);
    void sendQuerysNow();
    void setDelayTime();



};