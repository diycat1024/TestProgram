
#pragma once
#include "redis_pool.h"

class DBIORedis
{
public:
	DBIORedis();
	virtual ~DBIORedis();

	bool InitPool(const std::string& redis_addr, const std::size_t& port, const std::string& pwd, int conn_timeout, std::size_t pool_size, std::size_t pool_max_size);

	void ReleasePool();
	void CheckStatus();
	// String
	bool Set(const std::string& key, const std::string& field, const std::string& value);
	const char* Get(const std::string& key, const std::string& field);

	// Hash
	bool HSet(const std::string& key, const std::string& field, const std::string& value);
	bool HGet(const std::string& key, const std::string& field, std::string& reponse);

	
private:
	static const int pool_size = 50; //默认给定的对象池中的大小

	std::string     redis_ip_;
	std::uint32_t   redis_port_;
	std::uint32_t   time_out_;
	std::string     redis_pwd_;

	RedisPoolPtr pool_;
};
