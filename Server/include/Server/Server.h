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
#include <SensorMessage/SensorMessage.h>
#include <ServerListener/ServerListener.h>



// =============================================================================
namespace DachaServer
{
    class Server
    {
    public:
        enum State{
            Constructing,
            FailedToConstruct,
            Created,
            Started,
            StartListen,
            Stopped,
            ListenStopped,
            Destroyed,
        };
        using ServerStateChangedCallback = std::function<void (const State)>;
        using ErrorCallBack = std::function<void (const std::string&)>;
        using DataReciveCallBack = std::function<void (const SensorMessage&)>;
    public:
        Server(
            const int port,
            const int backlog,
            const ServerStateChangedCallback& onServerStateChangedCallback,
            const ErrorCallBack& onErorrCallback,
            const DataReciveCallBack& onDataRecivedCallback
        );
        ~Server();
        Server() = delete;
        Server(const Server &) = delete;
        Server &operator=(const Server &) = delete;

    public:
        // начать принимать соединения
        void start();
        // закончить прием соединений
        void stop();
        // возращает состояние сервера
        State state() const { return m_state;}
        // не уверен что нужно но может быть 
        // возращает количество обрабатываемых соединений
        size_t numberOfConnections();

    public:
        void addListner(ServerListener*);
        void eraseListner(ServerListener*);

    private:
        void acceptLoop();
        void reciveData(SocketDescriptorType sock);

    private:
        std::mutex m_listenersVecMutex;
        std::vector<ServerListener*> m_listenersVec;

    private:
        socket_wrapper::Socket m_listeningSocket;
        struct sockaddr_in m_addres;
        struct pollfd m_pollStruct;
    
    private:
        // колбек при изминении состояния сервера
        ServerStateChangedCallback onServerStateChangedCallback;
        // колбек если произошла какая-то ошибка
        ErrorCallBack onErrorCallback;
        // колбек после успешного приема данных
        DataReciveCallBack onDataRecivedCallback;

    private:
        // флаг для остановки 
        std::atomic<bool> m_isStop;
        //текущее состояние сервера
        std::atomic<State> m_state;
        // поток приема соединений
        std::thread m_listenerThread;
        std::mutex m_mutexForMap;
        std::condition_variable m_condtion;
        // дескриптор сокета асоциированный с потокм
        // в котором происходи прием данных из него
        std::unordered_map<SocketDescriptorType, std::thread> m_connectionThreadsMap;
    };

}// end of namesapce