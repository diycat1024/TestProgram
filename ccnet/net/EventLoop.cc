#include "EventLoop.h"

EventLoop::EventLoop(/* args */)
: quit_(true),
looping_(false)
{
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop()
{
    while(!quit_)
    {
        
    }
}