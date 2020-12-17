#include <iostream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EPOLL_SIZE 5000
#define BUF_SIZE 1024


std::list<int> clients;

int addfd(int epollfd, int fd, bool enable_et)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if (enable_et)
        ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)| O_NONBLOCK);
}

int sendBroadcastmessage(int fd)
{
    char buf[BUF_SIZE] = {0};
    
    printf("read from (clientfd %d)\n", fd);
    int len = recv(fd,buf, sizeof(buf), 0);
    if (len <= 0)
    {
        close(fd);
        clients.remove(fd);

    }else
    {
        char response[BUF_SIZE] = {0};

        sprintf(response,"send frome (clientfd %d), %s\n", fd, buf);
        for (auto &it : clients)
        {
            
            len = send(it, response, strlen(response), 0);
            if (len <= 0)
            {
                perror("error\n");
                exit(-1);
            }
        }
    }
    
}

int main()
{
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family  =  AF_INET;
    addr.sin_port = htons(8883);

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    bind(listener, (struct sockaddr*)&addr, sizeof(addr));
    listen(listener, 5);
    int epfd = epoll_create(EPOLL_SIZE);
    printf("create epfd %d\n",  epfd);

    static struct epoll_event events[EPOLL_SIZE];
    addfd(epfd, listener, true);

    while (1)
    {
        int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if  (epoll_events_count < 0)
        {
            printf("epoll faile");
            break;
        }
        printf("epoll_events_count= %d\n", epoll_events_count);

        for (int i = 0; i < epoll_events_count; i ++)
        {
            int sockfd = events[i].data.fd;
            if (sockfd == listener)
            {
                struct sockaddr_in  client_addr;
                socklen_t  client_length = sizeof(struct sockaddr_in);
                int clientfd = accept(listener, (struct sockaddr*)&client_addr,&client_length);

                printf("client connected from: %s: %d, clitndfd: %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), clientfd);
                addfd(epfd, clientfd, true);////把这个新的客户端添加到内核事件列表

                clients.push_back(clientfd);
                printf("Add new clientfd = %d to epoll\n", clientfd);
                printf("Now there are %d clients int the chat room\n", (int)clients.size());

                printf("welcome message\n");
                char buf[BUF_SIZE]= {0};
                sprintf(buf,"welcome message: %d", clientfd);
                int ret = send(clientfd, buf, strlen(buf), 0);
                if(ret < 0) { perror("send error"); exit(-1); }


            }
            else{
                int ret = sendBroadcastmessage(sockfd);
                if(ret < 0) { perror("send error"); exit(-1); }
                
            }
        }
    }
    close(listener);//关闭socket
    close(epfd);//关闭内核   不在监控这些注册事件是否发生
    return 1;
};