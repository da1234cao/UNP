#include "log.hpp"

#include <sys/socket.h>
#include <bits/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include <iostream>
#include <string>

#define LISTENQ 1024
#define BUFF_SIZE 1024

using namespace std;

int main(void)
{
    BOOST_LOG_TRIVIAL(info) << "start server.";

    string server_port = "10000";

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to create a listening socket.";
        return -1;
    }
  
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(stoi(server_port));

    bind(listenfd, (sockaddr *)&servaddr, sizeof(servaddr));

    if(listen(listenfd, LISTENQ) < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to set up the listening socket.";
        return -1;
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    int nfds = -1; // 记录最大的套接字+1

    while(1) {
        FD_SET(listenfd, &readfds); // 将监听套接字加入读描述符集

        // 永远等待下去，直到存在套接字可读
        nfds = listenfd > nfds-1 ? listenfd+1 : nfds;
        int n = select(nfds, &readfds, nullptr, nullptr, nullptr);

        if(n < 0) {
            BOOST_LOG_TRIVIAL(error) << "Select error...";
            return -1;
        }

        for(int fd=0; fd<nfds; fd++) {
            if(FD_ISSET(fd, &readfds)) {
                if(fd == listenfd) { // 监听套接字已完成的连接数不为0 --> 监听套接字准备好读
                    int client_sockfd = accept(listenfd, nullptr, nullptr);
                    FD_SET(client_sockfd, &readfds); // 将客户端套接字加入读描述符集
                    nfds = client_sockfd > nfds-1 ? client_sockfd+1 : nfds;
                } else {
                    char buffer[BUFF_SIZE] = {0};
                    int readbytes = read(fd, buffer, BUFF_SIZE);
                    if(readbytes > 0) {
                        cout << "receive data: " << buffer;
                    } else {
                        close(fd); // 这里需要注意，对于Tcp，能否根据返回read的返回值为零，来作为关闭socket的依据？
                        FD_CLR(fd, &readfds);
                        BOOST_LOG_TRIVIAL(info) << "Close socket fd: " << fd;
                    }
                }
            }
        }
    }

    close(listenfd);
}