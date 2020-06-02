
#include "redis_manage.h"
RedisManage::RedisManage(/* args */)
{
    redisClient_.reset(new DBIORedis());
}

RedisManage::~RedisManage()
{
}

RedisManage& RedisManage::instance()
{
    static RedisManage redisManage_;
    return redisManage_;
}

bool RedisManage::InitPool(const std::string& redis_addr, const std::size_t& port, const std::string& pwd, int conn_timeout, std::size_t pool_size, std::size_t pool_max_size)
{
    return redisClient_->InitPool(redis_addr, port, pwd, conn_timeout, pool_size, pool_max_size);
}



bool RedisManage::Set(const char* key, const char* field, const char* value)
{
    return redisClient_->Set(key, field, value);
}

const char* RedisManage::Get(const char* key, const char* field)
{
    return redisClient_->Get(key, field);
}


bool RedisManage::HSet(const char* key, const char* field, const char* value)
{
    return redisClient_->HSet(key, field, value);
}

bool RedisManage::HGet(const char* key, const char* field, std::string& value)
{
     return redisClient_->HGet(key, field, value);
}

void RedisManage::CheckStatus()
{
    redisClient_->CheckStatus();
}

void RedisManage::HGetAll(const std::string& key, std::map<std::string,std::string>& reponse)
{
    redisClient_->HGetAll(key, reponse);
}