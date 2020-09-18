#include <signal.h>
#include <string.h>
#include "libevent/event.h"
#include "libevent/event2/listener.h"

#define PORT 8881
static const char MESSAGE[] = "Hello world!";

void conn_writecb(struct bufferevent *bev, void *ctx)
{
	struct evbuffer* output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0)
	{
		printf("flushed answer\n");
		bufferevent_free(bev);
	}
}

void conn_readcb(struct bufferevent *bev, void *ctx)
{
	char read_msg[256] = { 0 };
	memset(read_msg, 0, sizeof(read_msg));

	struct evbuffer* input = bufferevent_get_input(bev);
	size_t sz = evbuffer_get_length(input);
	if (sz > 0)
	{
		bufferevent_read(bev, read_msg, sz);
		printf("str: %s\n", read_msg);

		bufferevent_write(bev, read_msg, strlen(read_msg));
	}
}


void conn_eventcb(struct bufferevent *bev, short events, void *ctx)
{
	if (events & BEV_EVENT_EOF)
	{
		printf("Connection close.\n");
	}
	else if (events & BEV_EVENT_ERROR)
	{
		printf("Got an error on the connection: %s\n", strerror(errno));
	}

	bufferevent_free(bev);
}

int main() 
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	struct event_base* base = event_base_new();

	struct sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof addr_in);
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(8881);
	addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");

	struct bufferevent* bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
	if (bev == nullptr)
	{
		fprintf(stderr, "socket init faile!\n");
		return -1;
	}
	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, nullptr);

	int flag = bufferevent_socket_connect(bev, (struct sockaddr*) &addr_in, sizeof(addr_in));
	if (flag == -1)
	{
		fprintf(stderr, "connect faile!\n");
		return 1;
	}
	bufferevent_enable(bev, EV_READ | EV_WRITE);

	event_base_dispatch(base);
	event_base_free(base);

#ifdef WIN32
	WSACleanup();
#endif

	printf("done\n");
	return 1;
}