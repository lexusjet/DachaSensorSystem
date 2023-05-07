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
            Constructing,
            Created,
            Started,
            StartListen,
            Stopped,
            ListenStopped,
            Destroyed,
        };
        using ServerStateChangedCallback = std::function<void (const State)>;
        using ErrorCallBack = std::function<void (const std::string&)>;
        using DataReciveCallBack = std::function<void (const MessageHeader&)>;
    public:
        Server(
            int port,
            int backlog,
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
        State state() const { return m_state;}


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
        // колбек при изминении состояния сервера
        ServerStateChangedCallback onServerStateChangedCallback;
        // колбек если произошла какая-то ошибка
        ErrorCallBack onErrorCallback;
        // колбек после успешного приема данных
        DataReciveCallBack onDataRecivedCallback;

    private:
        // флаг для остановки 
        std::atomic<bool> isStop;
        //
        std::atomic<State> m_state;
        // поток приема соединений
        std::thread listenerThread;
        std::mutex mutexForMap;
        std::condition_variable condtion;
        // дескриптор сокета асоциированный с потокм
        // в котором происходи прием данных из него
        std::unordered_map<SocketDescriptorType, std::thread> connectionThreadsMap;
    };

}// end of namesapce