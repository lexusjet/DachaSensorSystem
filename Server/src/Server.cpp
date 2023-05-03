#include <Server/Server.h>

#include <iostream>

using namespace dachaServer;

Server::Server(
    int port,
    int backlog,
    const DataReciveCallBack& onDataRecivedCallback,
    const ErrorCallBack& onErorrCallback) 
    : 
        listeningSocket(std::move(socket_wrapper::Socket(AF_INET, SOCK_STREAM, 0))),
        onDataRecivedCallback(onDataRecivedCallback),
        onErrorCallback(onErorrCallback),
        isStop(false)
{
    addres.sin_family = AF_INET;
    addres.sin_port = htons(port);
    addres.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(listeningSocket,(struct sockaddr*) &addres, sizeof(addres)) < 0){
        onErorrCallback(std::string("bind erorr"));
        return;
    }

    if(listen(listeningSocket, backlog) == -1){
        onErorrCallback(std::string("listen start eror"));
        return;
    }

    pollStruct.events = POLLIN;
    pollStruct.fd = static_cast<int>(listeningSocket);
}

Server::~Server(){
    isStop = true;
    if(listenerThread.joinable())
        listenerThread.join();
    std::unique_lock<std::mutex> lock(mutexForMap);
    condtion.wait(lock, [&](){return connectionThreadsMap.empty();}); 
}

void Server::acceptLoop()
{

    while(!isStop){
        if(poll(&pollStruct, 1, 1000) == 0) 
            continue;
        int newConnection = accept(listeningSocket, nullptr, nullptr);
        //TODO: check connection errors

        std::unique_lock<std::mutex> lock(mutexForMap);
        condtion.wait(lock, [](){return true;});
        connectionThreadsMap.emplace(newConnection,
            std::thread(
                &Server::reciveData,
                this,
                std::move(socket_wrapper::Socket(newConnection))
            )
        );
        condtion.notify_all();
        
        pollStruct.revents = 0;
    }
}

void dachaServer::Server::start()
{
    isStop = false;
    listenerThread = std::thread(&Server::acceptLoop, this);
}

void dachaServer::Server::stop()
{
    isStop = true;
}


void Server::reciveData(socket_wrapper::Socket connection)
{
    // char* data[sizeof(MessageHeader)];
    MessageHeader message;
    const int timeout = 1000;
    int bytesRead = 0;
    size_t recived = 0;

    int pollResult = 0;
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = static_cast<int>(connection);

    while(recived < sizeof(MessageHeader)){
        pollResult =  poll(&pfd, 1, timeout);
        if(pollResult != 1) 
            break;
        bytesRead = recv(connection,
            &message+recived,
            sizeof(MessageHeader) - recived,
            0
        );
        if(bytesRead <= 0){
            pollResult = -1;
            break;
        }
        recived += bytesRead;
    }
    if(!isStop){
        if(pollResult != 1){
            std::string errorMassage;
            if(pollResult == 0)
                errorMassage = "poll timeout";
            else
                errorMassage = "connection error ";
            onErrorCallback(errorMassage);
        }
        else
            onDataRecivedCallback(message);
    }

    std::unique_lock<std::mutex> lock(mutexForMap);
    condtion.wait(lock, [](){return true;});
    connectionThreadsMap[connection].detach();
    connectionThreadsMap.erase(connection);
    condtion.notify_all();
}
