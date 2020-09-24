#include "TcpServer.h"

#ifdef MACOS
#endif

#define PORT 8881
#define MAX_BUFFER 48000

int main() 
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

    TcpConnection conn;
    conn.init();
    conn.start();

    printf("server done...\n");
#ifdef WIN32
	WSACleanup();
#endif

	printf("done\n");
	return 1;
}
