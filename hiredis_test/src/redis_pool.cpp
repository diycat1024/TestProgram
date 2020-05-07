#include "redis_conn.h"

RedisConn::RedisConn():ctx_(nullptr)
{
	redis_ip_ = "127.0.0.1";
	redis_port_ = 6379;
	redis_pwd_ = "";
	time_out_ = 2000;
	status_ = false;
}

RedisConn::~RedisConn()
{
	Release();
}

bool RedisConn::Init(const std::string& redis_addr, const std::size_t& port, const std::string& pwd,
	int conn_timeout)
{
	redis_ip_ = redis_addr;
	redis_port_ = port;
	redis_pwd_ = pwd;
	time_out_ = conn_timeout;
	return true;
}

bool RedisConn::RedisConnect()
{
	bool bRet = false;
	if (NULL != ctx_) {
		redisFree(ctx_);
		ctx_ = NULL;
	}

	ctx_ = ConnectWithTimeout();
	if (NULL == ctx_) {
		bRet = false;
	}
	else {
		bRet = auth();
		status_ = bRet;
	}

	return bRet;
}


bool RedisConn::RedisReConnect()
{
	if (NULL == ctx_) {
		return false;
	}

	bool bRet = false;
	redisContext* tmp_ctx = ConnectWithTimeout();
	if (NULL == tmp_ctx) {
		bRet = false;
	}
	else {
		redisFree(ctx_);
		ctx_ = tmp_ctx;
		bRet = auth();
	}

	status_ = bRet;
	return bRet;
}

bool RedisConn::auth()
{
	bool bRet = false;
	if (0 == redis_pwd_.length()) {
		bRet = true;
	}
	else {
		redisReply* reply = static_cast<redisReply*>(redisCommand(ctx_, "AUTH %s", redis_pwd_.c_str()));
		if ((NULL == reply) || (strcasecmp(reply->str, "OK") != 0)) {
			bRet = false;
		}
		else {
			bRet = true;
		}
		freeReplyObject(reply);
	}

	return bRet;
}


redisContext* RedisConn::ConnectWithTimeout()
{
	struct timeval timeoutVal;
	timeoutVal.tv_sec = time_out_;
	timeoutVal.tv_usec = 0;
	redisContext* ctx = NULL;
	ctx = redisConnectWithTimeout(redis_ip_.c_str(), redis_port_, timeoutVal);
	if (NULL == ctx || ctx->err) {
		if (NULL != ctx) {
			redisFree(ctx);
			ctx = NULL;
		}
	}

	return ctx;
}

void RedisConn::Release()
{
	if (ctx_ != NULL)
		redisFree(ctx_);

	ctx_ = NULL;
	status_ = false;
}

bool RedisConn::Ping()
{
	if (ctx_ == NULL) return false;

	redisReply* reply = static_cast<redisReply*>(redisCommand(ctx_, "PING"));
	bool bRet = (NULL != reply) && (reply->str) && (strcasecmp(reply->str, "PONG") == 0);

	if (bRet) freeReplyObject(reply);

	status_ = bRet;
	return bRet;
}

bool RedisConn::ExecuteResonse(std::string& response, const char* format, ...)
{
	if (ctx_ == NULL) return NULL;

	/*std::vector<char> buf;
	buf.resize(10000);
	va_list st;
	va_start(st, format);
	vsprintf(&buf[0], format, st);
	va_end(st);
	std::string buf_str(&buf[0]);*/

	va_list args;
	va_start(args, format);
	redisReply* reply = static_cast<redisReply*>(redisCommand(ctx_, format, args));
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
		redisReply* rly0 = NULL;
		std::vector<std::string > params;
		ParamRly(reply, params);
		return false;
	}
	else
	{
		response = "Undefine Reply Type";
		return false;
	}
}

redisReply* RedisConn::ExecuteCmd(const char* format, ...)
{
	if (ctx_ == NULL) return NULL;

	va_list args;
	va_start(args, format);
	redisReply* reply = static_cast<redisReply*>(redisCommand(ctx_, format, args));
	va_end(args);

	return reply;
}

void RedisConn::ParamRly(redisReply* rly, std::vector<std::string>& params)
{
	redisReply* rly0 = NULL;
	if (rly->type == REDIS_REPLY_ARRAY)
	{
		for (int i = 0; i < (int)rly->elements; i++) {
			rly0 = rly->element[i];
			if (rly0 == NULL) continue;

			if (rly0->type == REDIS_REPLY_ARRAY) {
				ParamRly(rly0, params);
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



// Á¬½Ó³Ø
RedisPool::RedisPool()
{

}

RedisPool::~RedisPool()
{
	ReleaseRedisConn();
}

bool RedisPool::InitPool(const std::string& redis_addr, const std::size_t& port, const std::string& pwd, int conn_timeout, std::size_t pool_size, std::size_t pool_max_size)
{
	redis_ip_ = redis_addr;
	redis_port_ = port;
	redis_pwd_ = pwd;
	time_out_ = conn_timeout;
	pool_size_ = pool_size;
	pool_max_size_ = pool_max_size_;

	for (int i = 0; i < pool_size; i++) {
		RedisConn* pRedisconn = new RedisConn;
		if (NULL == pRedisconn) continue;

		pRedisconn->Init(redis_ip_, redis_port_, redis_pwd_, time_out_);
		if (pRedisconn->RedisConnect())
			conn_pool_.push(pRedisconn);
		else
			delete pRedisconn;
	}

	return true;
}

RedisConn* RedisPool::GetRedisConn()
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (conn_pool_.empty()) return NULL;
	RedisConn* pRedisconn = pRedisconn = static_cast<RedisConn*>(conn_pool_.front());
	conn_pool_.pop();
	return pRedisconn;

}

void RedisPool::FreeRedisConn(RedisConn* redis_conn)
{
	if (NULL != redis_conn) {
		std::lock_guard<std::mutex> lock(mutex_);
		conn_pool_.push(redis_conn);
	}
}

void RedisPool::ReleaseRedisConn()
{
	std::lock_guard<std::mutex> lock(mutex_);
	for (int i = 0; i < conn_pool_.size(); i++)
	{
		conn_pool_.pop();
	}
}

void RedisPool::CheckStatus()
{
	std::lock_guard<std::mutex> lock(mutex_);
	for (int i = 0; i < conn_pool_.size(); i++)
	{
		RedisConn* conn = static_cast<RedisConn*>(conn_pool_.front());
		if (!conn->Ping())
			conn->RedisReConnect();
	}
}