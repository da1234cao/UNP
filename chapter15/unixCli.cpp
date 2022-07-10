#include "global.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    struct sockaddr_un servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH); // 小心这里缓冲区溢出

    if(connect(sockfd, (const sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "fail to connect...\n");
        return 0;
    }

    char sendBuf[MAXLINE] = {0};
    char recvBuf[MAXLINE] = {0};
    while(fgets(sendBuf, MAXLINE-1, stdin)) {
        write(sockfd, sendBuf, strlen(sendBuf));
        int n = read(sockfd, recvBuf, MAXLINE-1);
        if(n > 0) {
            fputs(recvBuf, stdout);
        }
    }

    return 0;
}