#pragma once
#include <string.h>
#ifndef WIN32
#include <arpa/inet.h>
#include <zconf.h>
#include <sys/select.h>
#endif
#include <memory>
#include <thread>
#include <mutex>

#include "event2/util.h"
#include "libevent/event.h"

static const char MESSAGE[] = "Hello world!";
#define PORT 8881

class TcpConnection
{
public:
    TcpConnection();
    ~TcpConnection();

    static void conn_cb(struct bufferevent* bev, short events, void* ctx);
    static void conn_readcb(struct bufferevent* bev, void* ctx);
    static void conn_writecb(struct bufferevent* bev, void* ctx);

    int32_t init();
    void    start();
private:
    std::shared_ptr<std::thread> thread_;
    std::mutex mutex_;

    struct event_base* base_;
    struct bufferevent* bev_;
};

