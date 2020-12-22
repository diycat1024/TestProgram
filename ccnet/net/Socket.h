#pragma once
#include <netinet/in.h>
#include <stdio.h>
#include "TcpAddr.h"

class Socket
{
public:
    Socket(const TcpAddr& addr);
    ~Socket();
    int fd() const {return fd_;};
private:
    int fd_;
};
