#include "redis_conn.h"

RedisConn::RedisConn():pool_(NULL)
{
	acl::acl_cpp_init();
}

RedisConn::~RedisConn()
{
}

bool RedisConn::InitPool(const char* redis_addr, std::size_t count,
	int conn_timeout, int rw_timeout)
{
	pool_ = new acl::redis_client_pool(redis_addr, count);
	pool_->set_timeout(conn_timeout, rw_timeout);
	return true;
}

bool RedisConn::Set(const char* key, const char* field, const char* value)
{
    // 创建 redis string 类型的命令操作类对象，同时将连接类对象与操作类
	// 对象进行绑定
	acl::redis_string string_operation((acl::redis_client*)pool_->peek());
 
	// 添加 K-V 值至 redis-server 中
	if (string_operation.set(key, value) == false)
	{
		// const acl::redis_result* res = string_operation.get_result();
		// printf("set key: %s error: %s\r\n",
		// 	key, res ? res->get_error() : "unknown error");
		return false;
	}
 
	// 需要重置连接对象的状态
	string_operation.clear();
    pool_->put(string_operation.get_client(), true);
    return true;

}

const char* RedisConn::Get(const char* key, const char* field)
{
    // 创建 redis string 类型的命令操作类对象，同时将连接类对象与操作类
	// 对象进行绑定
	acl::redis_string string_operation((acl::redis_client*)pool_->peek());

	// 从 redis-server 中取得对应 key 的值
    acl::string buf;    
	if (string_operation.get(key, buf) == false) return NULL;
	

	// 需要重置连接对象的状态
	string_operation.clear();
    pool_->put(string_operation.get_client(), true);

    return buf.c_str();
}

bool RedisConn::HSet(const char* key, const char* field, const char* value)
{
	acl::redis_hash hash_operation((acl::redis_client*)pool_->peek());
	if (hash_operation.hset(key, field, value) == false) return false;
	
	hash_operation.clear();
    pool_->put(hash_operation.get_client(), true);
    return true;
}

bool RedisConn::HGet(const char* key, const char* field, std::string& value)
{
	acl::redis_hash hash_operation((acl::redis_client*)pool_->peek());
	acl::string buf;
	if (hash_operation.hget(key, field, buf) == false) return false;
	
	hash_operation.clear();
    pool_->put(hash_operation.get_client(), true);

	value = buf.c_str();
	return true;
}