#pragma once

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_child(int signo)
{
    int pid;
    int stat;
    while(pid = waitpid(-1, &stat, WNOHANG) > 0) {
        ;
    }
    return;
}