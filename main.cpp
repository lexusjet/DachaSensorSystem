
#include <iostream>
#include <functional>
#include <string>
#include <signal.h>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include <Server/Server.h>
#include <MessageHeader/MessageHeader.h>

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

    std::function<void(MessageHeader)> onDataRecivedCallback =
        [](MessageHeader data){ std::cout << data << std::endl; };
    std::function<void(std::string)> onErorrCallback =
        [](std::string erorr){ std::cout << erorr << std::endl; };

    dachaServer::Server server(3425, 5,
                               onDataRecivedCallback, onErorrCallback);
    
    
    server.start();
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [](){return static_cast<bool>(isStop);});
    server.stop();

    return 0;
}