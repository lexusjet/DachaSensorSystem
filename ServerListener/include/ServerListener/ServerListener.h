#pragma once
#include <SensorMessage/SensorMessage.h>

class ServerListener
{
    
public:
    virtual void notify(const SensorMessage&) = 0;
    virtual ~ServerListener() = default;
};