#include <signal.h>
#include <string.h>
#include <errno.h>
#include <vector>

#include "libevent/event.h"
#include "libevent/event2/listener.h"
#ifdef MACOS
#endif

#define PORT 8881
#define MAX_BUFFER 48000
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
    else
    {
	  printf("a new conn \n");
    }
	bufferevent_free(bev);
}

void listen_cb(struct evconnlistener * listener, evutil_socket_t fd, struct sockaddr * sa, int socklen, void * user_data)
{
	struct event_base * base = evconnlistener_get_base(listener);
	struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		fprintf(stderr, "Error construct bufferevent\n");
		return;
	}
	
	bufferevent_setcb(bev, conn_readcb, nullptr, conn_eventcb, nullptr);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

	//bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

void signal_cb(evutil_socket_t, short, void * user_data)
{
	struct event_base* base = static_cast<event_base*>(user_data);
	struct timeval delay = { 2,0 };
	
	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);


}

int main() 
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	struct event_base* base = event_base_new();
	struct evconnlistener* listen;
	struct event* signal_event;

	struct sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof addr_in);
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(PORT);

	listen = evconnlistener_new_bind(base,listen_cb,(void*)base, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&addr_in,sizeof(addr_in) );

	if (!listen)
	{
		fprintf(stderr, "could not create listen\n");
		return -1;
	}

	signal_event = evsignal_new(base, SIGINT, signal_cb, (void*)base);
	if (!signal_event || event_add(signal_event, nullptr) < 0)
	{
		fprintf(stderr, "Could not create/add a signal event! \n");
		return -1;
	}

	printf("Accept...\n");
	event_base_dispatch(base);

    printf("server done...\n");
	evconnlistener_free(listen);
	event_free(signal_event);
	event_base_free(base);

#ifdef WIN32
	WSACleanup();
#endif

	printf("done\n");
	return 1;
}
