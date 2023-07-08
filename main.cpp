
#include <iostream>
#include <functional>
#include <string>
#include <signal.h>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include <Server/Server.h>
#include <SensorMessage/SensorMessage.h>
#include <SensorMessageValidator/SensorMessageValidator.h>
#include <SensorMessageConverter/SensorMessageConverter.h>
#include <MessageHandler/MessageHandler.h>
#include <SensorMessageValidator/SensorMessageValidator.h>
#include <DatabaseClient/DatabaseClient.h>
#include <DatabaseConnector/DatabaseConnector.h>
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

SensorMessageValidator createSensorMessageValidator()
{
    std::vector<size_t> m_verions = {1};
    
    std::vector<size_t> place_1 = {1, 2, 3};
    std::vector<size_t> place_2 = {4, 5, 6, 7};
    std::vector<size_t> place_3 = {8, 9, 10, 11};
    std::vector<size_t> place_4 = {12, 13};
    std::unordered_map<size_t, std::vector<size_t>> locaPlace;
    locaPlace[1] = place_1;
    locaPlace[2] = place_2;
    locaPlace[3] = place_3;
    locaPlace[4] = place_4;

    std::vector<size_t> m_dataTypesVec = {1};
    std::vector<int> m_extenionsVec = {0};
    std::vector<int> m_reservesVec = {0};

    return SensorMessageValidator(
        m_verions,
        locaPlace,
        m_dataTypesVec,
        m_extenionsVec,
        m_reservesVec
    );
}

SensorMessageConverter createSensorMessageConverter()
{
    return SensorMessageConverter(
        std::unordered_map<size_t, std::string>(), 
        std::unordered_map<size_t, std::string>(), 
        std::unordered_map<size_t, std::string>() 
    );
}

int main(int argc, const char **argv)
{
    
    if(signal(SIGINT, signalFunction) == SIG_ERR){ return 0;};
    
    
    
    SensorMessageValidator validator(createSensorMessageValidator());
    SensorMessageConverter converter(createSensorMessageConverter());
    
    MessageHandler messageHandler(
        validator,
        converter, 
        [](const SensorMessage, std::string a)
        {
            std::cout << a << std::endl;
        }
    );

    std::string addres = "127.0.0.1";
    std::string port = "3306";
    std::string dataBaseName = "test_dacha";
    std::string userName = "root";
    std::string password = "";
    DatabaseClient dataBaseClient(
        addres,
        port,
        userName,
        password,
        dataBaseName,
        [](const std::string ){std::cout << "insereted" << std::endl;},
        [](const std::string a, DataBaseException e){ std::cout<< "controled " << e.what() << std::endl;}
    );

    messageHandler.setDatabaseClient(&dataBaseClient);

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
    
    

    DachaServer::Server::DataReciveCallBack onDataRecivedCallback =
        [&messageHandler](const SensorMessage& message)
        {
            messageHandler.handleMessage(message);
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

    return 0;
}