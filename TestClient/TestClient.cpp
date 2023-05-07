#include "TestClient.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <string>
#include <Logger.h>

TestClient::TestClient(const std::string& ip, const int port, const MessageHeader& message)
    : m_messageHeader(message)
{
    struct sockaddr_in addr;

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 0)
    {
        perror("socket");
        exit(1);
    }


    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // или любой другой порт...
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) 
    {
        close(m_socket);
        return;
    }

    if(connect(m_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(m_socket);
        return;
    }
}

TestClient::~TestClient()
{
    close(m_socket);
}

void TestClient::sendMessage()
{
    size_t total = 0;
    int n;
    // LOG("sending: \n" << m_messageHeader << "\n");

    while(total < MESSEGE_SIZE)
    {
        n = send(m_socket, &m_messageHeader + total, MESSEGE_SIZE-total, 0);
        if(n == -1) { break; }
        total += n;
    }
    
    
}