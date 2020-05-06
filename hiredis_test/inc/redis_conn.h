
#include <stdio.h>
#include "redis_pool.h"
#include <memory>

class RedisConn : public std::enable_shared_from_this<RedisConn>
{
private:
    /* data */
public:
    RedisConn();
    ~RedisConn();
    // String
    bool Set(const std::string& key, const std::string& field, const std::string& value);
    const char* Get(const std::string& key, const std::string& field) ;

    // Hash
    bool HSet(const std::string& key, const std::string& field, const std::string& value);
    bool HGet(const std::string& key, const std::string& field, std::string& value);

    bool InitPool(const std::string& redis_addr, std::size_t port, std::size_t count,
	int conn_timeout, int rw_timeout); 
	redisContext* connectRedis();

public:
	std::shared_ptr<KGRedisClient> kgClient_;
};

