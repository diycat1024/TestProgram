#include <iostream>
#include "redis_manage.h"
#include <thread>
#include <Windows.h>
void test()
{
		// 创建 redis 连接池对象
	acl::redis_client_pool pool("192.168.0.172:6379", 20);
	// 设置连接 redis 的超时时间及 IO 超时时间，单位都是秒
	pool.set_timeout(20, 20);
	size_t t= pool.get_count();
	acl::redis_client* conn = (acl::redis_client*) pool.peek();
	if (conn == NULL)
	{
		printf("conn is null");
		return;
	}
	acl::redis_string string_operation(conn);
	const char* value = "test_value";
	
	const char* key = "user:base";
	// 添加 K-V 值至 redis-server 中
	if (string_operation.set(key, value) == false)
	{
		const acl::redis_result* res = string_operation.get_result();
		printf("set key: %s error: %s\r\n",
			key, res ? res->get_error() : "unknown error");
		return ;
	}
	printf("set key: %s ok!\r\n", key);
 
	// 需要重置连接对象的状态
	string_operation.clear();

// return true;
}


int main()
{
	RedisManage redisManage;
	
	redisManage.InitPool("192.168.0.212:6379", 30, 20,20);

	// if (redisManage.HSet("user:last","bigant1", "bbbbb"))
	// {
	// 	printf("插入成功");
	// }
	auto threadFunc =  [&]() {
		for (int i=0; i< 10000000000; i++) {
			std::string rets;
			bool ret = redisManage.HGet("user:base","DE61F1EF-FC53-968F-118A-9378CE037D85@1569505695714", rets);
			printf("chnegg：%s\n", rets);
		}
	};
	for (int b= 0; b < 100; b ++) {
		std::thread t1(threadFunc);
		t1.detach();
	}

	std::thread t2( [&]() {
		while(true)
		{
			Sleep(1);
		}
	});
	t2.join();
	return 1;
}
