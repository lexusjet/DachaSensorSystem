#pragma once

#include <SensorMessage/SensorMessage.h>


class TestClient
{
private:
    SensorMessage m_messageHeader;
    int m_socket;
public:
    void sendMessage();
    TestClient(const std::string& ip, const int port, const SensorMessage& message);
    ~TestClient();
};



