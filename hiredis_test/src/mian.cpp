#include <iostream>
#include "redis_manage.h"
#include <thread>
#include <Windows.h>


int main()
{
	RedisManage redisManage;
	
	redisManage.InitPool("192.168.0.212",6379, 30, 20,20);

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
