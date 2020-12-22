#pragma once
#include <string>
#include <string.h>
#include <netinet/in.h>


class TcpAddr
{
public:
    TcpAddr(unsigned short port);
    ~TcpAddr();
    
    struct sockaddr_in addr() const {return addr_;}
    sa_family_t family() const {return addr_.sin_family;};
    
private:
    unsigned short port_;
    const char* ip_;
    struct sockaddr_in addr_;
};
