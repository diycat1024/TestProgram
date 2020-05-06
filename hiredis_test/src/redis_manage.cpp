
#include "redis_manage.h"
RedisManage::RedisManage(/* args */)
{
    _redisConn.reset(new RedisConn());
}

RedisManage::~RedisManage()
{
}

RedisManage& RedisManage::instance()
{
    static RedisManage redisManage_;
    return redisManage_;
}

bool RedisManage::InitPool(const char* redis_addr, std::size_t port, std::size_t count,
	int conn_timeout, int rw_timeout)
{
    return _redisConn->InitPool(redis_addr, port,count, conn_timeout, rw_timeout);
}



bool RedisManage::Set(const char* key, const char* field, const char* value)
{
    return _redisConn->Set(key, field, value);
}

const char* RedisManage::Get(const char* key, const char* field)
{
    return _redisConn->Get(key, field);
}


bool RedisManage::HSet(const char* key, const char* field, const char* value)
{
    return _redisConn->HSet(key, field, value);
}

bool RedisManage::HGet(const char* key, const char* field, std::string& value)
{
     return _redisConn->HGet(key, field, value);
}
