
#include "acl/lib_acl.h"
#include "acl_cpp/lib_acl.hpp"
#include <memory>

class RedisConn : public std::enable_shared_from_this<RedisConn>
{
private:
    /* data */
public:
    RedisConn();
    ~RedisConn();

    // String
    bool Set(const char* key, const char* field, const char* value);
    const char* Get(const char* key, const char* field) ;

    // Hash
    bool HSet(const char* key, const char* field, const char* value);
    bool HGet(const char* key, const char* field, std::string& value);

    bool InitPool(const char* redis_addr, std::size_t count,
	int conn_timeout, int rw_timeout); 
private:
    acl::redis_client_pool* pool_;
};

