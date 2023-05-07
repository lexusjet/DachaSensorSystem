
#include <iostream>
#include <functional>
#include <string>
#include <signal.h>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include <Server/Server.h>
#include <MessageHeader/MessageHeader.h>

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
    
    dachaServer::Server::DataReciveCallBack onDataRecivedCallback =
        [](MessageHeader data){ };
    dachaServer::Server::ErrorCallBack onErorrCallback =
        [](std::string erorr){ std::cout << erorr << std::endl; };
    dachaServer::Server::ServerStateChangedCallback onServerStateChangedCallback =
        [](dachaServer::Server::State state){ 
                switch (state)
                {
                    case dachaServer::Server::Created:
                        break;
                    case dachaServer::Server::Stopped:
                        break;
                    case dachaServer::Server::StartListen:
                        break;
                    case dachaServer::Server::Started:
                        break;
                    default:
                        break;
                }            
            };
    
    


    dachaServer::Server server(3425, 5,
                            onServerStateChangedCallback,
                            onErorrCallback,
                            onDataRecivedCallback
                        );
    
    
    server.start();
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [](){return static_cast<bool>(isStop);});
    server.stop();

    return 0;
}