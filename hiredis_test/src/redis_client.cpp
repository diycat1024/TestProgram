#include "redis_client.h"
DBIORedis::DBIORedis():pool_(new RedisPool())
{
}
DBIORedis::~DBIORedis()
{
}

bool DBIORedis::InitPool(const std::string& redis_addr, const std::size_t& port, const std::string& pwd, int conn_timeout, std::size_t pool_size, std::size_t pool_max_size)
{
	return pool_->InitPool(redis_addr, port, pwd, conn_timeout, pool_size, pool_max_size);
}

void DBIORedis::ReleasePool()
{
	pool_->ReleaseRedisConn();
}

void DBIORedis::CheckStatus()
{
	pool_->CheckStatus();
}


bool DBIORedis::Set(const std::string& key, const std::string& field, const std::string& value)
{
	std::string reponse;
	RedisConnPrt conn = pool_->GetRedisConn();
	if (!conn) return false;

	std::string commond = "set " + key + " " + field + " " + value;
	bool ret = conn->ExecuteResonse(reponse, commond.c_str());
	//bool ret = conn->ExecuteResonse(reponse, "set %s %s %s", key.c_str(), field.c_str(), value.c_str());
	pool_->FreeRedisConn(conn);
	return ret;
}



const char* DBIORedis::Get(const std::string& key, const std::string& field)
{
	std::string reponse;
	RedisConnPrt conn = pool_->GetRedisConn();
	if (!conn) return false;

	std::string commond = "get " + key;
	bool ret = conn->ExecuteResonse(reponse, commond.c_str());
	//bool ret = conn->ExecuteResonse(reponse, "get %s", key.c_str());
	pool_->FreeRedisConn(conn);
	return reponse.c_str();

}

bool DBIORedis::HSet(const std::string& key, const std::string& field, const std::string& value)
{
	std::string reponse;
	RedisConnPrt conn = pool_->GetRedisConn();
	if (!conn) return false;

	std::string commond = "hset " + key + " " + field + " " + value;
	bool ret = conn->ExecuteResonse(reponse, commond.c_str());
	//bool ret = conn->ExecuteResonse(reponse, "hset %s %s %s", key.c_str(), field.c_str(), value.c_str());
	pool_->FreeRedisConn(conn);
	return ret;
}

bool DBIORedis::HGet(const std::string& key, const std::string& field, std::string& reponse)
{
	RedisConnPrt conn = pool_->GetRedisConn();
	if (!conn) return false;

	std::string commond = "hget " + key + " " + field;
	bool ret = conn->ExecuteResonse(reponse, commond.c_str());
	/*bool ret = conn->ExecuteResonse(reponse, "hget %s %s", key.c_str(), field.c_str());*/
	pool_->FreeRedisConn(conn);
	return ret;
}

bool DBIORedis::HKeys(const std::string& key, std::vector<std::string>& response)
{
	RedisConnPrt conn = pool_->GetRedisConn();
	if (!conn.get()) return false;

	std::string commond = "hkeys " + key + '\0';
	bool ret = conn->ExecuteVector(response, commond.c_str());
	/*bool ret = conn->ExecuteResonse(reponse, "hget %s %s", key.c_str(), field.c_str());*/
	pool_->FreeRedisConn(conn);
	return ret;	
}

bool DBIORedis::HGetAllByKeys(const std::string& key, std::map<std::string, std::string>& response)
{
	RedisConnPrt conn = pool_->GetRedisConn();
	if (!conn.get()) return false;

	std::vector<std::string> akeys;
	if (!HKeys(key, akeys)) 	return false;

	std::string commond, res;
	for (auto& akey : akeys)
	{
 		commond = "hget " + key + ' ' + akey;
		res = "";
		if(conn->ExecuteResonse(res, commond.c_str()))
		{
			response[akey] = res;
		}
	}
	pool_->FreeRedisConn(conn);
	return true;
}


bool DBIORedis::HGetAll(const std::string& key, std::map<std::string, std::string>& response)
{
	RedisConnPrt conn = pool_->GetRedisConn();
	if (!conn.get()) return false;

	std::string commond = "hgetall " + key + '\0';
	bool ret = conn->ExecuteMap(response, commond.c_str());
	pool_->FreeRedisConn(conn);
	return ret;
}

