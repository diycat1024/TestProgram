#pragma once
#include <netinet/in.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <vector>

#include "Channel.h"

class EPollPoller
{
public:
    EPollPoller(/* args */);
    ~EPollPoller();

    int init(int port);
    void addfd(int epollfd, bool enable_et);
    void  poll();
private:
    /* data */
    int sock_fd_;
    int epfd_;
    std::vector<struct epoll_event> events_;
    ChannleMap channels_;
};
