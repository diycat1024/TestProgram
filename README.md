## 测试程序集合
目录：

[acl-redis-test]() acl对redis的使用

[hiredis_test]() hiredis对reids的使用

[mongo-c-test]() mongo-c-driver的使用

[mongo-cxx-test]() mongo-cxx-driver的使用

[catfly/client]() win客户端pingpang协议

[catfly/server]() win服务器pingpang协议

[catfly/SmartPointer]() 实现智能指针

libevent_test/timer libevent 定时器

libevent_test/server    libevent服务器例子
    
libevent_test/client libevent 客户客户端例子



## 包管理
统一使用vcpkg包管理。

acl是手动编译的：
[acl-github](https://github.com/acl-dev/acl/tree/master/lib_acl_cpp/samples/redis)

 
## TODO LIST
- [x] 实现智能指针
- [x] UNIX AF_UNIX通信server/client
- [x] libevent网络库的简单使用
- [ ]  win网络传输库
- [ ] linux网络传输库
- [ ] 跨平台的网络传输库
 
libevent_test/server 单聊/群聊/发送文件/代理
libevent_test/client 单聊/群聊/发送文件/代理
