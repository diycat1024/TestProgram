#include "Channel.h"
#include "EPollPoller.h"

Channel::Channel(EventLoop *loop, int sockfd)
: loop_(loop),
events_(0),
revents_(0),
fd_(sockfd)
{
}

Channel::~Channel()
{
}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::remove()
{
    loop_->removeChannel(this);
}

bool Channel::isWriting() const
{
    return events_ &  EPOLLOUT;
}

bool Channel::isReading() const
{
    return events_ & (EPOLLIN | EPOLLPRI);
}

void Channel::enableConnecting()
{
    events_ |= EPOLLIN;
    update();
}

void Channel::enableReading()
{
    events_ |= (EPOLLIN | EPOLLPRI);
    update();
}

void Channel::disableReading()
{
    //todo why
    events_ &= ~(EPOLLIN | EPOLLPRI);
    update();
}

void Channel::enableWriting()
{
    printf("Channel, enableWriting\n");
    events_ |= EPOLLOUT;
    update();
}

void Channel::disableWriting()
{
    //todo why
    events_ &= ~EPOLLOUT;
    update();
}


void Channel::handleEvent()
{
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        printf("handleEvent close: %d\n",fd_);
        if (close_callback_) close_callback_();
    }
    if (revents_ & ( EPOLLIN | EPOLLPRI | EPOLLRDHUP)) 
    {
        if (read_callback_) read_callback_();
    }

    if (revents_ & EPOLLOUT)
    {
        if (write_callback_) write_callback_();
    }
    if (revents_ & (EPOLLERR))
    {
        printf("handleEvent error: error_callback_");
        if (error_callback_) error_callback_();
    }

}