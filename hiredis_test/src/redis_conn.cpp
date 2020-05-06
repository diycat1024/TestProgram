#include "redis_conn.h"

RedisConn::RedisConn():kgClient_(nullptr)
{
}

RedisConn::~RedisConn()
{
}

bool RedisConn::InitPool(const std::string& redis_addr, std::size_t port, std::size_t count,
	int conn_timeout, int rw_timeout)
{
	kgClient_.reset(new KGRedisClient(redis_addr, port, ""));
	return true;
}

redisContext* RedisConn::connectRedis()
{
	return kgClient_->CreateContext();
}

bool RedisConn::Set(const std::string& key, const std::string& field, const std::string& value)
{
	redisContext* redisContext = connectRedis();
	if (redisContext == NULL) return false;

	std::string command = "set " + key;

	redisReply* r = (redisReply*)redisCommand(redisContext, command.c_str());

   freeReplyObject(r);
   
   return true;
}
 
	

const char* RedisConn::Get(const std::string& key, const std::string& field)
{
	redisContext* redisContext = connectRedis();
	if (redisContext == NULL) return NULL;

	std::string command = "get " + key;

	redisReply* r = (redisReply*)redisCommand(redisContext, command.c_str());

   freeReplyObject(r);
   
   return r->str;
   
}

bool RedisConn::HSet(const std::string& key, const std::string& field, const std::string& value)
{
	redisContext* redisContext = connectRedis();
	if (redisContext == NULL) return NULL;

	std::string command = "hset " + key + " " + field + " " + value;

	redisReply* r = (redisReply*)redisCommand(redisContext, command.c_str());

	freeReplyObject(r);
	return true;
}

bool RedisConn::HGet(const std::string& key, const std::string& field, std::string& value)
{
	
	redisContext* redisContext = connectRedis();
	if (redisContext == NULL) return NULL;

	std::string command = "hget " + key + " " + field;

	redisReply* r = (redisReply*)redisCommand(redisContext, command.c_str());
	if (r->type == REDIS_REPLY_STRING)
		value = r->str;
	freeReplyObject(r);
	return true;
}