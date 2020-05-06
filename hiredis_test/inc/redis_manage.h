

#include "redis_conn.h"
#include <string>
#include <memory>

typedef std::shared_ptr<RedisConn> RedisConnPtr;
class RedisManage{
public:
    RedisManage(/* args */);
    ~RedisManage();

    RedisManage& instance();

    bool InitPool(const char* redis_addr, std::size_t port, std::size_t count,
	int conn_timeout, int rw_timeout);

    //String
    bool Set(const char* key, const char* field, const char* value);
    const char* Get(const char* key, const char* field);

    //Hash
    bool HSet(const char* key, const char* field, const char* value);
    bool HGet(const char* key, const char* field, std::string& value);
private:
    /* data */
    RedisConnPtr _redisConn;
};
