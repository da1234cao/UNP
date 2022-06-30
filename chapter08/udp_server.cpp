#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1024

int main(void) {
    const char *local_port = "10000";

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        fprintf(stderr, "Failed to create the client socket. Procedure");
        return -1;
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(local_port));

    bind(sockfd, (const sockaddr*)&servaddr, sizeof(servaddr));

    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);

    char recev_buf[MAXLINE] = {0};
    while(1) {
        bzero(&clientaddr, sizeof(clientaddr));
        bzero(&recev_buf, sizeof(recev_buf));
        clientaddr_len = sizeof(clientaddr); // 这个初始化不能设置为零，应当设置成结构体的大小。否则revfrom
        int n = recvfrom(sockfd, recev_buf, sizeof(recev_buf), 0, (struct sockaddr*)&clientaddr, &clientaddr_len);
        
        if(sendto(sockfd, recev_buf, n, 0, (const sockaddr*)&clientaddr, clientaddr_len) < 0) {
            fprintf(stderr, "Failed to send back data.");
            fprintf(stderr, "Current client addr is %s:%u.\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
            return -1;
        }
    }
}