#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1024

int main(void) {
    const char *server_port = "10000";
    const char *servet_ip = "127.0.0.1";

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        fprintf(stderr, "Failed to create the client socket. Procedure");
        return -1;
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, servet_ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(atoi(server_port));

    char send_buf[MAXLINE] = {0};
    char recev_buf[MAXLINE] = {0};
    while(1) {
        bzero(send_buf, sizeof(send_buf));
        fprintf(stdout, "need to be sent: ");
        fgets(send_buf, sizeof(send_buf), stdin);
        
        int n = sendto(sockfd, send_buf, strlen(send_buf), 0, (const sockaddr*)&servaddr, sizeof(servaddr));
        if(n < 0) {
            fprintf(stderr, "Failed to senda to data");
            return -1;
        }

        recvfrom(sockfd, recev_buf, sizeof(recev_buf), 0, NULL, NULL);
        fprintf(stdout, "received data: %s", recev_buf);
    }
}