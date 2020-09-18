#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib") 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <io.h>
#include <tchar.h>
#endif

#include <iostream>
#include "libevent/event.h"
struct event ev;
struct timeval tv;
void time_cb(evutil_socket_t fd, short event, void *argc)
{
	printf("timer wakeup\n");
	event_add(&ev, &tv); // reschedule timer
}

int main()
{
#ifdef WIN32 
	WSADATA wsa_data; 
	WSAStartup(0x0201, &wsa_data);
#endif 
	struct event_base *base = event_base_new();
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	evtimer_set(&ev, time_cb, nullptr);
	event_base_set(base, &ev);
	event_add(&ev, &tv);
	event_base_dispatch(base);

	event_base_free(base);
#ifdef WIN32 
	WSACleanup();
#endif 
	return 1;
}