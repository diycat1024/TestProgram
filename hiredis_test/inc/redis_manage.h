#include <string>
#include <memory>
#include "redis_client.h"

class RedisManage{
public:
    RedisManage(/* args */);
    ~RedisManage();

    RedisManage& instance();

    bool InitPool(const std::string& redis_addr, const std::size_t& port, const std::string& pwd, int conn_timeout, std::size_t pool_size, std::size_t pool_max_size);
    void CheckStatus();

    //String
    bool Set(const char* key, const char* field, const char* value);
    const char* Get(const char* key, const char* field);

    //Hash
    bool HSet(const char* key, const char* field, const char* value);
    bool HGet(const char* key, const char* field, std::string& value);
private:
    /* data */
    std::shared_ptr<DBIORedis> redisClient_;
};
