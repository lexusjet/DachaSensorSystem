#pragma once

#include <MessageHeader/MessageHeader.h>


class TestClient
{
private:
    MessageHeader m_messageHeader;
    int m_socket;
public:
    void sendMessage();
    TestClient(const std::string& ip, const int port, const MessageHeader& message);
    ~TestClient();
};



