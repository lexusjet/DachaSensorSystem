
#include <Server/Server.h>

#include <iostream>
#include <condition_variable>
#include <thread>
#include <vector>
#include <TestClient.h>
#include <Logger.h>
#include <chrono>
#include <unordered_map>

void clientThreadWorker(const std::string& ip, const int port, const std::vector<MessageHeader>& dataToSendVector)
{
    //TODO: here we will create test client
    LOG("started "<< __func__ << " #" << static_cast<int>(dataToSendVector.back().getLocal()));
    for (const auto& it: dataToSendVector)
    {
        TestClient client(ip, port, it);
        client.sendMessage();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    LOG("ended "<< __func__ << " #" << dataToSendVector.back().getLocal());
}


std::vector<MessageHeader> createDataToSendVector(const size_t id, const size_t count)
{
    std::vector<MessageHeader> result;
    static int32_t data = 1000;

    for (size_t i = 0; i < count; i++)
    {   
        MessageHeader messege;
        messege.setVersion(1);
        messege.setLocal(id);
        messege.setNumberInLocalization(i);
        messege.setDataType(1);
        messege.setData(data);
        messege.setExtension(0);
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
    // 
    std::cout << "Test Statr" << std::endl; 

    std::condition_variable cv;
    std::mutex recivedDataMapMutex;

    std::vector<std::thread> clientThreadsVector;
    std::vector<std::vector<MessageHeader>> dataToSendVectors;
    std::unordered_map<size_t, std::unordered_map<size_t, MessageHeader>> recivedDataMap;


    std::string ip = "127.0.0.1";
    int port = 3332;
    int backlog = 10;
    int messageCount = 1000;
    dachaServer::Server server(port, backlog,
        [&](const dachaServer::Server::State state)
        {
            switch (state)
            {
            case dachaServer::Server::Created: 
                
                break;
            case dachaServer::Server::Started: 
                
                break;
            case dachaServer::Server::StartListen: 
                for (int i = 0; i < backlog; i++)
                {
                    dataToSendVectors.emplace_back(createDataToSendVector(i, messageCount));
                }
                
                for(const auto& it: dataToSendVectors)
                {
                    clientThreadsVector.emplace_back(std::thread(clientThreadWorker, ip, port, it));
                }
                
                cv.notify_all();
                break;
            case dachaServer::Server::Stopped: 
                
                break;
            case dachaServer::Server::ListenStopped: 
                
                break;
            case dachaServer::Server::Destroyed: 
                
                break;

            }

        },
        [](const std::string& str){},
        [&](const MessageHeader& message)
        {
            std::lock_guard lock(recivedDataMapMutex);
            recivedDataMap[message.getLocal()].emplace(message.getNumberInLocalization(), message);
            // LOG("Recieving: \n" << message << "\n");
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
            std::cout <<("Error recieved data isn`t consistent! 2") << std::endl;
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