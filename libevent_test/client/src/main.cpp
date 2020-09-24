#include "TcpConnection.h"

bool nostop = true;
int main() 
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

    while(nostop)
    {
      std::shared_ptr<TcpConnection> conn(new TcpConnection());
      conn->init(); 
      conn->start();
      sleep(3);
    }

#ifdef WIN32
	WSACleanup();
#endif

	printf("done\n");
	return 1;
}
