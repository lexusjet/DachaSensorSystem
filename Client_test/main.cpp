#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string>
#include <cstring>
#include <iostream>

#include <MessageHeader/MessageHeader.h>

using namespace std;

int sendMessege(int s, MessageHeader messege, int flags){
    size_t total = 0;
    int n;
    while(total < MESSEGE_SIZE)
    {
        n = send(s, &messege + total, MESSEGE_SIZE-total, flags);
        if(n == -1) { break; }
        total += n;
    }

    return (n==-1 ? -1 : total);

}
// не уверен что работает кореткно
MessageHeader reciveMessegeHeader(int s, int flags)
{
    MessageHeader messege;
    int bytesRead = 0;
    size_t recived = 0;

    while(recived < sizeof(messege)){
        bytesRead = recv(s, &messege+recived, sizeof(messege) - recived, flags);
        if(bytesRead <= 0) break;
        recived += bytesRead;
    }
    return messege;
}

// int sendall(int s, char *data, int len, int flags)
// {
//     int total = 0;
//     int n;
//     while(total < len)
//     {
//         n = send(s, data+total, len-total, flags);
//         if(n == -1) { break; }
//         total += n;
//     }

//     return (n==-1 ? -1 : total);
// }

// MessegeHeader reciveAll(int s, int flags)
// {
//     MessegeHeader messege;
//     char data[MESSEGE_SIZE];

//     int bytesRead = 0;
//     int recived = 0;

//     while(recived < sizeof(data)){
//         bytesRead = recv(s, data+recived, sizeof(data) - recived, flags);
//         if(bytesRead <= 0) break;
//         recived += bytesRead;
//     }
//     messege.setMessegeHeader(data);
//     return messege;
// }


int main()
{

    MessageHeader messege;
    messege.setVersion(1);
    messege.setLocal(1);
    messege.setNumberInLocalization(1);
    messege.setDataType(1);
    messege.setData(int32_t(211));
    messege.setExtension(int32_t(0));
    messege.setReserv(int32_t(0));

    std::cout << messege << std::endl;
    std::cout <<std::endl;

    // char buf[MESSEGE_SIZE];
    //memcpy(buf, messege.getMessegeHeader().c_str(), sizeof(buf));


    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }


    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    sendMessege(sock, messege, 0);

    usleep(300000);
    close(sock);
    return 0;
}
