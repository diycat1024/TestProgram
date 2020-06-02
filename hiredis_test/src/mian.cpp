#include <iostream>
#include "redis_manage.h"
#include <thread>


int main()
{
	RedisManage redisManage;
	
	redisManage.InitPool("192.168.0.172",6379,"",1,50,100);
	redisManage.CheckStatus();
	std::map<std::string,std::string> response;
	redisManage.HGetAll("user:base", response);

	// if (redisManage.HSet("user:last","bigant1", "bbbbb"))
	// {
	// 	printf("插入成功");
	// }
	auto threadFunc =  [&]() {
		for (int i=0; i< 1000; i++) {
			std::string rets;
			bool ret = redisManage.HGet("user:base","DE61F1EF-FC53-968F-118A-9378CE037D85@1569505695950", rets);
			if (!ret)
			{
				printf("chnegg：%s\n", rets);
			}
			Sleep(5);
		}
	};
	for (int b= 0; b < 150; b ++) {
		std::thread t1(threadFunc);
		t1.detach();
	}

	std::thread t2( [&]() {
		while(true)
		{
			Sleep(1000);
		}
	});
	t2.join();
	return 1;
}
