
#include <iostream>
#include <functional>
#include <string>
#include <signal.h>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include <Server/Server.h>
#include <SensorMessage/SensorMessage.h>
#include <MessageHandler/MessageHandler.h>

#include "Logger.h"

std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> isStop(false);

void signalFunction(int sig)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [](){return !isStop;});
    isStop = true;
    cv.notify_all();
}

int main(int argc, const char **argv)
{
    if(signal(SIGINT, signalFunction) == SIG_ERR){ return 0;};
    
    DachaServer::Server::ErrorCallBack onErorrCallback =
        [](std::string erorr){ std::cout << erorr << std::endl; };
    DachaServer::Server::ServerStateChangedCallback onServerStateChangedCallback =
        [](DachaServer::Server::State state){ 
                switch (state)
                {
                    case DachaServer::Server::Created:
                        break;
                    case DachaServer::Server::Stopped:
                        break;
                    case DachaServer::Server::StartListen:
                        break;
                    case DachaServer::Server::Started:
                        break;
                    default:
                        break;
                }            
            };
    
    
    MessageHandler messageHendler;
    messageHendler.start();
    DachaServer::Server::DataReciveCallBack onDataRecivedCallback  =
        [&messageHendler](const SensorMessage& message)
        {
            messageHendler.addMessage(message);
        };

    DachaServer::Server server(3425, 5,
                            onServerStateChangedCallback,
                            onErorrCallback,
                            onDataRecivedCallback
                        );
    
    
    server.start();
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [](){return static_cast<bool>(isStop);});
    server.stop();
    messageHendler.stop();

    return 0;
}