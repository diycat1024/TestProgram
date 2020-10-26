#pragma once
#include <string.h>
#include <signal.h>
#ifndef WIN32
#include <arpa/inet.h>
#include <zconf.h>
#include <sys/select.h>
#endif
#include <memory>
#include <thread>
#include <mutex>
#include <vector>

#include "event2/util.h"
#include "libevent/event.h"
#include "libevent/event2/listener.h"

static const char MESSAGE[] = "Hello world!";
#define PORT 8881
#define MAX_BUFFER 48000

class TcpConnection
{
public:
    TcpConnection();
    ~TcpConnection();

    static void conn_cb(struct bufferevent* bev, short events, void* ctx);
    static void conn_readcb(struct bufferevent* bev, void* ctx);
    static void conn_writecb(struct bufferevent* bev, void* ctx);
    static void listen_cb(struct evconnlistener * listener, evutil_socket_t fd, struct sockaddr * sa, int socklen, void * user_data);
    static void signal_cb(evutil_socket_t, short, void * user_data);

    int32_t init();
    void    start();
private:
    std::shared_ptr<std::thread> thread_;
    std::mutex mutex_;

    struct event_base* base_;
    struct evconnlistener* listen_;
    struct event* signal_event_;

    struct bufferevent* bev_;
};

