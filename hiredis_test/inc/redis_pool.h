#ifndef __REDIS_POOL_H__
#define __REDIS_POOL_H__
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <memory>
#include <mutex>
#include <queue>
#ifdef WIN32
#include <time.h>
#include <WinSock2.h>
#else
#include <sys/time.h>
#endif
#include "hiredis/hiredis.h"

#define strcasecmp _stricmp
class KGRedisClient
{
public:
	KGRedisClient(std::string ip, int port, std::string password, int timeout = 2000);
	virtual ~KGRedisClient();

	redisContext* CreateContext();

	//   bool ExecuteCmd_spop(const char *cmd, size_t len, std::string &response);
	bool ExecuteCmd_spop(std::string &response, const char* format, ...);

	//   redisReply* ExecuteCmd(const char *cmd, size_t len);
	redisReply* ExecuteCmd(const char* format, ...);
	bool ExecuteCmd(std::string &response, const char* format, ...);
private:
	int m_timeout;
	int m_serverPort;
	std::string m_setverIp;
	std::string m_password;
	//   CCriticalSection m_lock;
	std::mutex _mutex;
	std::queue<redisContext *> m_clients;
	time_t m_beginInvalidTime;
	static const int m_maxReconnectInterval = 3;
	void ReleaseContext(redisContext *ctx, bool active);
	bool CheckStatus(redisContext *ctx);
};

#endif
