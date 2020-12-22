#pragma once

class Channel;
class EPollPoller;

class EventLoop
{
public:
    EventLoop(/* args */);
    ~EventLoop();

    void  loop();
private:
    bool quit_;
    bool looping_;
    int epfd_;
};
