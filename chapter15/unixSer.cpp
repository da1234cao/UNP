#include "global.h"
#include "single.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>

// 整体缺少函数返回值的检查

void str_echo(int connfd);

int main(void)
{
    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    struct sockaddr_un cliaddr, servaddr;
    bzero(&cliaddr, sizeof(cliaddr));
    bzero(&servaddr, sizeof(servaddr));

    unlink(UNIXSTR_PATH);
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH); // 小心这里缓冲区溢出
    bind(listenfd, (const sockaddr*)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    signal(SIGCHLD, sig_child);

    while(1) {
        int clilen = sizeof(cliaddr);
        int connfd = accept(listenfd, (sockaddr*)&cliaddr, (socklen_t*)&clilen);
        int childpid;
        if((childpid = fork()) == 0) {
            close(listenfd);
            str_echo(connfd);
            return 0;
        }
        close(connfd);
    }
}

void str_echo(int connfd)
{
    char buf[MAXLINE] = {0};
    int n;
    while((n = read(connfd, buf, MAXLINE)) > 0) {
        write(connfd, buf, n);
    }
}