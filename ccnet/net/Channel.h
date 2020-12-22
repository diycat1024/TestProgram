#pragma once
#include "EventLoop.h"
#include <map>

class EPollPoller;

class Channel
{
public:
    Channel(EventLoop *loop);
    ~Channel();
private:
    EventLoop* loop_;
    
};
typedef std::map<int, Channel*> ChannleMap;