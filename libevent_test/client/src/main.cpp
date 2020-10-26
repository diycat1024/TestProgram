#include "TcpConnection.h"
#include <thread>
#include <map>

bool nostop = true;
int main() 
{
#ifdef WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    std::map<int, TcpConnPtr> conn_map; 
    int n = 0;
    std::thread thread1([&](){
                        n++;
                        TcpConnPtr conn(new TcpConnection());
                        conn->init(); 
                        printf("1n=%d\n", n);
                        conn_map[n] = conn;
                        conn->start();
                        });

    while(true)
    {
        for(auto itr: conn_map)
            itr.second->sendMsg("22222222222222222222222\n");    
        sleep(3);

    }
    thread1.join();
#ifdef WIN32
    WSACleanup();
#endif

    printf("done\n");
    return 1;
}
