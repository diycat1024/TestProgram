#include "TcpServer.h"

TcpConnection::TcpConnection()
{
    thread_ = nullptr;
    base_ = event_base_new();
}

TcpConnection::~TcpConnection()
{
    event_base_free(base_);
    evconnlistener_free(listen_);
    event_free(signal_event_);

}


void TcpConnection::conn_cb(struct bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_EOF)
    {
        printf("Connection close.\n");
    }
    else if (events & BEV_EVENT_ERROR)
    {
        printf("Got an error on the connection: %s\n", strerror(errno));
    }
    else
    {
        printf("a new conn \n");
    }
    bufferevent_free(bev);
}

void TcpConnection::conn_readcb(struct bufferevent* bev, void* ctx)
{

    struct evbuffer* input = bufferevent_get_input(bev);

    size_t sz = evbuffer_get_length(input);
    printf("sz = %d\n", sz);

    std::vector<char> read;
    read.resize(MAX_BUFFER);
    if (sz > 0)
    {
        bufferevent_read(bev, &read.front(), sz);
    }


    printf("str: %s\n", (const char*)&read.front());

    //struct evbuffer* output = bufferevent_get_output(bev);
    //evbuffer_add_buffer(output, input);
    return;
}

void TcpConnection::conn_writecb(struct bufferevent* bev, void* ctx)
{

    struct evbuffer* output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0)
    {
        printf("flushed answer\n");
        bufferevent_free(bev);
    }
}


void TcpConnection::listen_cb(struct evconnlistener * listener, evutil_socket_t fd, struct sockaddr * sa, int socklen, void * user_data)
{
    struct event_base * base = evconnlistener_get_base(listener);
    struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        fprintf(stderr, "Error construct bufferevent\n");
        return;
    }

    bufferevent_setcb(bev, conn_readcb, nullptr, conn_cb, nullptr);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    //bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}
void TcpConnection::signal_cb(evutil_socket_t, short, void * user_data)
{
    struct event_base* base = static_cast<event_base*>(user_data);
    struct timeval delay = { 2,0 };

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    event_base_loopexit(base, &delay);


}

int32_t TcpConnection::init()
{
    struct sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof addr_in);
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(PORT);

    listen_= evconnlistener_new_bind(base_,listen_cb,(void*)base_, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&addr_in,sizeof(addr_in) );
    if (!listen)
    {
        fprintf(stderr, "could not create listen\n");
        return -1;
    }
    signal_event_ = evsignal_new(base_, SIGINT, signal_cb, (void*)base_);
    if (!signal_event_ || event_add(signal_event_, nullptr) < 0)
    {
        fprintf(stderr, "Could not create/add a signal event! \n");
        return -1;
    }


}

void TcpConnection::start()
{
    event_base_dispatch(base_);
}
