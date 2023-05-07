
#include <Server/Server.h>

#include <iostream>
#include <condition_variable>
#include <thread>
#include <vector>
#include <TestClient.h>
#include <Logger.h>
#include <chrono>
#include <unordered_map>

void clientThreadWorker(const std::string& ip, const int port, const std::vector<SensorMessage>& dataToSendVector)
{
    // LOG("started "<< __func__ << " #" << static_cast<int>(dataToSendVector.back().getLocal()));
    for (const auto& it: dataToSendVector)
    {
        TestClient client(ip, port, it);
        client.sendMessage();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // LOG("ended "<< __func__ << " #" << dataToSendVector.back().getLocal());
}


std::vector<SensorMessage> createDataToSendVector(const size_t id, const size_t count)
{
    std::vector<SensorMessage> result;
    static int32_t data = 1000;

    for (size_t i = 0; i < count; i++)
    {   
        SensorMessage messege;
        messege.setVersion(1);
        messege.setLocation(id);
        messege.setNumberInLocation(1);
        messege.setDataType(1);
        messege.setData(data);
        messege.setExtension(i);
        messege.setReserv(0);
        result.emplace_back(messege);
        
        data += 100;
    }
    
    return result;
}

int main(int argc, const char** argv) 
{
    // порядок такой мы запускаем сервер 
    // получаем коллбек что он начал слушать сокет
    // запускаем процессы/потоки на подключение и передачу данных
    // скажем будет единовременно будет максимальное число на порте
    std::cout << "Test Statr" << std::endl; 

    std::condition_variable cv;
    std::mutex recivedDataMapMutex;

    std::vector<std::thread> clientThreadsVector;
    std::vector<std::vector<SensorMessage>> dataToSendVectors;
    std::unordered_map<size_t, std::unordered_map<size_t, SensorMessage>> recivedDataMap;


    std::string ip = "127.0.0.1";
    int port = 3332;
    size_t backlog = 10;
    int messageCount = 1000;
    DachaServer::Server server(port, backlog,
        [&](const DachaServer::Server::State state)
        {
            switch (state)
            {
            case DachaServer::Server::Created: 
                
                break;
            case DachaServer::Server::Started: 
                
                break;
            case DachaServer::Server::StartListen: 
                for (size_t i = 0; i < backlog; i++)
                {
                    dataToSendVectors.emplace_back(createDataToSendVector(i, messageCount));
                }
                
                for(const auto& it: dataToSendVectors)
                {
                    clientThreadsVector.emplace_back(std::thread(clientThreadWorker, ip, port, it));
                }
                
                cv.notify_all();
                break;
            case DachaServer::Server::Stopped: 
                
                break;
            case DachaServer::Server::ListenStopped: 
                
                break;
            case DachaServer::Server::Destroyed: 
                
                break;

            }

        },
        [](const std::string& str)
        {
            LOG(str);
        },
        [&](const SensorMessage& message)
        {
            std::lock_guard lock(recivedDataMapMutex);
            recivedDataMap[message.getLocation()].emplace(message.getExtension(), message);
            cv.notify_all();
        }
    );
    server.start();
    std::mutex mtx;
    std::unique_lock lock(mtx);
    cv.wait(lock, [&](){return clientThreadsVector.size() == backlog;});
    

    for(auto& it: clientThreadsVector)
        it.join();

    server.stop();

    if(recivedDataMap.size() != dataToSendVectors.size())
    {
        std::cout <<("Error recieved data isn`t consistent! 1") << std::endl;
        return 0;
    }
    for(const auto& it: recivedDataMap)
    {
        if(it.second.size() < dataToSendVectors.at(it.first).size())
        {
            std::cout <<"Error recieved data isn`t consistent! 2"<< std::endl;
            return 0;
        }
        for(const auto& itr: it.second)
        {
            
            if(itr.second != dataToSendVectors.at(it.first).at(itr.first))
            {
                std::cout <<("Error recieved data isn`t consistent! 3") << std::endl;
                return 0;
            }
        }
    }

    std::cout <<("Success recieved data is consistent!") << std::endl;
    
    return 0;
}