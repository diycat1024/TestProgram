#include "TcpConnection.h"

TcpConnection::TcpConnection()
{
    thread_ = nullptr;
    base_ = event_base_new();
    msg_buffer_.resize(4096);
}

TcpConnection::~TcpConnection()
{
    event_base_free(base_);
}

void TcpConnection::conn_cb(struct bufferevent* bev, short events, void* ctx)
{
    if (events & BEV_EVENT_EOF)
    {
        printf("Connection close.\n");
    }
    else if (events & BEV_EVENT_ERROR)
    {
        printf("Got an error on the connection: %s\n", strerror(errno));
    }
    printf("conn_eventcb\n");

    /*int i = 0;

      while (i < 1000)
      {
      i++;
      printf("i:%d\n", i);*/
    bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
    /*}*/

    /* bufferevent_free(bev); */
}
void TcpConnection::conn_readcb(struct bufferevent* bev, void* ctx)
{

    char read_msg[256] = { 0 };
    memset(read_msg, 0, sizeof(read_msg));

    struct evbuffer* input = bufferevent_get_input(bev);
    size_t sz = evbuffer_get_length(input);
    if (sz > 0)
    {
        bufferevent_read(bev, read_msg, sz);
        printf("str: %s\n", read_msg);

        bufferevent_write(bev, &read_msg, strlen(read_msg));
    }
}

void TcpConnection::conn_writecb(struct bufferevent* bev, void* ctx)
{

    struct evbuffer* output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0)
    {
        printf("flushed answer\n");
        /* bufferevent_free(bev); */
    }
}

int32_t TcpConnection::init()
{
    bev_ =  bufferevent_socket_new(base_, -1, BEV_OPT_CLOSE_ON_FREE);
    if (bev_ == nullptr)
    {
        printf("bev init error ");
        return -1;
    }
    bufferevent_setcb(bev_,conn_readcb, conn_writecb , conn_cb, nullptr);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);
    int flags = bufferevent_socket_connect(bev_,(struct sockaddr*)&addr, sizeof(addr));
    if (flags == -1)
    {
        printf("connect error!");
        return -1;
    }

    bufferevent_enable(bev_, EV_READ|EV_WRITE);
}

void TcpConnection::start()
{
    event_base_dispatch(base_);
}

bool TcpConnection::sendMsg(std::string msg)
{
    bufferevent_write(bev_,msg.c_str(), msg.length());
    printf("12312312");
}


