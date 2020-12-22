#include "EPollPoller.h"

EPollPoller::EPollPoller(/* args */)
: epfd_(::epoll_create(EPOLL_CLOEXEC))
{
}

EPollPoller::~EPollPoller()
{
}

int EPollPoller::init(int port)
{
    
    
}

void EPollPoller::addfd(int epollfd, bool enable_et)
{
    struct epoll_event ev;
    ev.data.fd = sock_fd_;
    ev.events = EPOLLIN;
    if (enable_et)
        ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_fd_, &ev);
    fcntl(sock_fd_, F_SETFL, fcntl(sock_fd_, F_GETFD, 0)| O_NONBLOCK);
}

void EPollPoller::poll()
{
    int epoll_events_count = ::epoll_wait(epfd_, &*events_.begin(), static_cast<int>(events_.size()), -1);
    if (epoll_events_count <= 0)
    {
        perror("epoll_events_count <=0");
        return;
    }
    for (int i=0; i < epoll_events_count; i ++)
    {
        
    }
}