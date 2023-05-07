#include <Server/Server.h>
#include <Logger.h>

#include <iostream>

using namespace DachaServer;

Server::Server(
    const int port,
    const int backlog,
    const ServerStateChangedCallback& onServerStateChangedCallback,
    const ErrorCallBack& onErorrCallback,
    const DataReciveCallBack& onDataRecivedCallback
    ) 
    : 
        m_listeningSocket(std::move(socket_wrapper::Socket(AF_INET, SOCK_STREAM, 0))),
        onServerStateChangedCallback(onServerStateChangedCallback),
        onErrorCallback(onErorrCallback),
        onDataRecivedCallback(onDataRecivedCallback),
        m_isStop(false),
        m_state(Constructing)

{
    m_addres.sin_family = AF_INET;
    m_addres.sin_port = htons(port);
    m_addres.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(m_listeningSocket,(struct sockaddr*) &m_addres, sizeof(m_addres)) < 0){
        onErorrCallback(std::string("Server bind erorr"));
        LOG("Server Bind error when creating server. Port " << port << " is occupied");
        m_state = FailedToConstruct;
        onServerStateChangedCallback(m_state);
        return;
    }

    if(listen(m_listeningSocket, backlog) == -1){
        onErorrCallback(std::string("Server listen start error"));
        LOG("Server listen start error");
        m_state = FailedToConstruct;
        onServerStateChangedCallback(m_state);
        return;
    }

    m_pollStruct.events = POLLIN;
    m_pollStruct.fd = static_cast<int>(m_listeningSocket);
    m_state = Created;
    onServerStateChangedCallback(m_state);
    LOG("Server created on port " << port);
}

Server::~Server(){
    m_isStop = true;
    if(m_listenerThread.joinable())
        m_listenerThread.join();
    std::unique_lock lock(m_mutexForMap);
    m_condtion.wait(lock, [&](){return m_connectionThreadsMap.empty();}); 
    m_state = Destroyed;
    onServerStateChangedCallback(m_state);
    LOG("Server successfully destroyed");
}

void Server::acceptLoop()
{
    LOG("Server start accepting connections on socket #" << static_cast<int>(m_listeningSocket));
    m_state = Started;
    onServerStateChangedCallback(m_state);
    while(!m_isStop){
        if(m_state != StartListen){
            m_state = StartListen;
            onServerStateChangedCallback(m_state);
        }
        if(poll(&m_pollStruct, 1, 1000) == 0) 
            continue;
        SocketDescriptorType newConnection = accept(m_listeningSocket, nullptr, nullptr);
        //TODO: check connection errors

        std::lock_guard<std::mutex> lock(m_mutexForMap);
        LOG("Server accept new connection on socket #" << newConnection);
        m_connectionThreadsMap.emplace(newConnection,
            std::thread(
                &Server::reciveData,
                this,
                newConnection
            )
        );
        
        m_pollStruct.revents = 0;
    }
    m_state = ListenStopped;
    onServerStateChangedCallback(m_state);
    LOG("Server stop accepting connections");
}

void DachaServer::Server::start()
{
    if(m_state != Created && m_state != ListenStopped){
        onErrorCallback("Server: can`t start work");
        return;
    }
    m_isStop = false;
    m_listenerThread = std::thread(&Server::acceptLoop, this);
    LOG("Server started");
}

void DachaServer::Server::stop()
{
    m_isStop = true;
    if(m_state == FailedToConstruct){
        onErrorCallback("Server: nothing to stop");
    }
    m_state = Stopped;
    onServerStateChangedCallback(Stopped);
    LOG("Server: stoped");
}

size_t DachaServer::Server::numberOfConnections() 
{
    std::lock_guard lock(m_mutexForMap);
    return m_connectionThreadsMap.size();
}

void Server::reciveData(SocketDescriptorType newConnection)
{
    
    socket_wrapper::Socket connection(newConnection);
    SensorMessage message;
    const int timeout = 1000;
    size_t recived = 0;

    int pollResult = 0;
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = static_cast<int>(connection);

    LOG("Server: start reciving data on socket #" << newConnection);
    while(recived < sizeof(SensorMessage) && !m_isStop){
        pollResult =  poll(&pfd, 1, timeout);
        if(pollResult != 1) 
            break;
        int bytesRead = recv(connection,
            &message+recived,
            sizeof(SensorMessage) - recived,
            0
        );
        if(bytesRead <= 0){
            pollResult = -1;
            break;
        }
        recived += bytesRead;
    }

    if(!m_isStop){
        if(pollResult != 1){
            std::string errorMassage;
            if(pollResult == 0){
                errorMassage = "poll timeout";
                LOG("Server: poll timeout on socket #" << newConnection);
            }
            else{
                errorMassage = "connection error ";
                LOG("Server: Connection error on socket #" << newConnection);
            }
            onErrorCallback(errorMassage);
        }
        else{
            LOG("data successfully read from the socket #" << newConnection);
            onDataRecivedCallback(message);
        }
    }
    
    std::lock_guard lock(m_mutexForMap);
    m_connectionThreadsMap[connection].detach();
    m_connectionThreadsMap.erase(connection);
    m_condtion.notify_all();
}
