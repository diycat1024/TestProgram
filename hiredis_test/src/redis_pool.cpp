#include "redis_pool.h"
KGRedisClient::KGRedisClient(std::string ip, int port, std::string password, int timeout)
{
	m_timeout = timeout;
	m_serverPort = port;
	m_setverIp = ip;
	m_password = password;
	m_beginInvalidTime = 0;
}
KGRedisClient::~KGRedisClient()
{
	//    CAutoLock autolock(m_lock);
	std::unique_lock <std::mutex> lck(_mutex);
	while (!m_clients.empty())
	{
		redisContext *ctx = m_clients.front();
		redisFree(ctx);
		m_clients.pop();
	}
}


void rly1(redisReply* rly, std::vector<std::string>& params)
{
	redisReply* rly0 = NULL;
	if (rly->type == REDIS_REPLY_ARRAY)
	{
		
		for (int i = 0; i < (int)rly->elements; i++) {
			rly0 = rly->element[i];
			if (rly0 == NULL) continue;

			if (rly0->type == REDIS_REPLY_ARRAY) {
				rly1(rly0, params);
			}
			else
			{
				params.push_back(rly0->str);
			}

		}
	}
	else
	{
		params.push_back(rly->str);
	}
}


bool KGRedisClient::ExecuteCmd(std::string &response, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	redisReply *reply = ExecuteCmd(format, args);
	va_end(args);

	if (reply == NULL) return false;
	std::shared_ptr<redisReply> autoFree(reply, freeReplyObject);
	if (reply->type == REDIS_REPLY_INTEGER)
	{
		response = std::to_string(reply->integer);
		return true;
	}
	else if (reply->type == REDIS_REPLY_STRING)
	{
		response.assign(reply->str, reply->len);
		return true;
	}
	else if (reply->type == REDIS_REPLY_STATUS)
	{
		response.assign(reply->str, reply->len);
		return true;
	}
	else if (reply->type == REDIS_REPLY_NIL)
	{
		response = "";
		return true;
	}
	else if (reply->type == REDIS_REPLY_ERROR)
	{
		response.assign(reply->str, reply->len);
		return false;
	}
	else if (reply->type == REDIS_REPLY_ARRAY)
	{
		//response = "Not Support Array Result!!!";

		redisReply* rly0 = NULL;
		std::vector<std::string > params;
		rly1(reply, params);
		/*for (int i = 0; i < (int)reply->elements; i++) {
			rly0 = reply->element[i];
			if (rly0 == NULL) continue;

			if (rly0->type == REDIS_REPLY_ARRAY) {
				for (int j = 0; j < (int)rly0->elements; j++) {
					rly1 = rly0->element[j];
					if (rly1 == NULL) continue;
					response = response + rly1->str + " ";
				}
			}
			else {
				printf("rly%d,type=%d,str=%s\r\n", i, rly0->type, rly0->str);
				response = response + rly0->str + " ";
			}
		}*/

		//std::shared_ptr<redisReply> autoFreeRly(rly0, freeReplyObject);

		return false;
	}
	else
	{
		response = "Undefine Reply Type";
		return false;
	}
}
redisReply* KGRedisClient::ExecuteCmd(const char* format, ...)
{
	va_list args;
	va_start(args, format);


	redisContext *ctx = CreateContext();
	if (ctx == NULL) return NULL;
	//  redisReply *reply = (redisReply*)redisCommand(ctx, "spop %b", cmd, len);
      //redisReply *reply = (redisReply*)redisCommand(ctx, "%s userinfo userid %s ", "hmset", "555234"); 
	redisReply* reply = (redisReply*)redisCommand(ctx, format, args);
	va_end(args);

	ReleaseContext(ctx, reply != NULL);
	return reply;
}

redisContext* KGRedisClient::CreateContext()
{
	{
		//        CAutoLock autolock(m_lock);
		std::unique_lock <std::mutex> lck(_mutex);
		if (!m_clients.empty())
		{
			redisContext *ctx = m_clients.front();
			m_clients.pop();
			return ctx;
		}
	}
	time_t now = time(NULL);
	if (now < m_beginInvalidTime + m_maxReconnectInterval) return NULL;
	struct timeval tv;
	tv.tv_sec = m_timeout / 1000;
	tv.tv_usec = (m_timeout % 1000) * 1000;;
	redisContext *ctx = redisConnectWithTimeout(m_setverIp.c_str(), m_serverPort, tv);
	if (ctx == NULL || ctx->err != 0)
	{
		if (ctx != NULL) redisFree(ctx);
		m_beginInvalidTime = time(NULL);

		return NULL;
	}
	redisReply *reply;
	std::string strReply = "AUTH ";
	strReply += m_password;
	reply = (redisReply*)redisCommand(ctx, strReply.c_str());
	 (reply);
	reply = NULL;
	printf("connect OK\n");
	return ctx;
}
void KGRedisClient::ReleaseContext(redisContext *ctx, bool active)
{
	if (ctx == NULL) return;
	if (!active) { redisFree(ctx); return; }
	//    CAutoLock autolock(m_lock);
	std::unique_lock <std::mutex> lck(_mutex);
	m_clients.push(ctx);
}
bool KGRedisClient::CheckStatus(redisContext *ctx)
{
	redisReply *reply = (redisReply*)redisCommand(ctx, "ping");
	if (reply == NULL) return false;
	std::shared_ptr<redisReply> autoFree(reply, freeReplyObject);
	if (reply->type != REDIS_REPLY_STATUS) return false;
	if (strcasecmp(reply->str, "PONG") != 0) return false;
	return true;
}