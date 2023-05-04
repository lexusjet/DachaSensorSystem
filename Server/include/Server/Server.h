#pragma once

#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <functional>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <Socket_class/socket_class.h>
#include <MessageHeader/MessageHeader.h>



// =============================================================================
namespace dachaServer
{

    class Server
    {
    public:
        enum State{
            Created,
            Stopped,
            StartListen,
            Started,

        };
        using DataReciveCallBack = std::function<void (MessageHeader)>;
        using ErrorCallBack = std::function<void (std::string)>;
        using ServerStateChangedCallback = std::function<void (State)>;
    public:
        Server(
            int port,
            int backlog,
            const DataReciveCallBack& onDataRecivedCallback,
            const ErrorCallBack& onErorrCallback,
            const ServerStateChangedCallback& onServerStateChangedCallback);
        ~Server();
        Server() = delete;
        Server(const Server &) = delete;
        Server &operator=(const Server &) = delete;

    public:
        // начать принимать соединения
        void start();
        // закончить прием соединений
        void stop();

    private:
        void acceptLoop();
        void reciveData(SocketDescriptorType sock);

        // void onReadDone(std::string);
        // void onReadError(std::string);

    private:
        socket_wrapper::Socket listeningSocket;
        struct sockaddr_in addres;
        struct pollfd pollStruct;
    
    private:
        // колбек после успешного приема данных
        DataReciveCallBack onDataRecivedCallback;
        // колбек если произошла какая-то ошибка
        ErrorCallBack onErrorCallback;
        // колбек при изминении состояния сервера
        ServerStateChangedCallback onServerStateChangedCallback;

    private:
        // флаг для остановки 
        std::atomic<bool> isStop;
        // поток приема соединений
        std::thread listenerThread;
        std::mutex mutexForMap;
        std::condition_variable condtion;
        // дескриптор сокета асоциированный с потокм
        // в котором происходи прием данных из него
        std::unordered_map<SocketDescriptorType, std::thread> connectionThreadsMap;
    };

}// end of namesapce