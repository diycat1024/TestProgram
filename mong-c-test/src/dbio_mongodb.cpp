#include "dbio_mongodb.h"


/*
 * dbio_mysql.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: root
 */

MongoDBConnectionPool::MongoDBConnectionPool()
{
	mongoc_init();
}


MongoDBConnectionPool::~MongoDBConnectionPool()
{
}

void MongoDBConnectionPool::mongodb_init(const char* url)
{
	m_strurl = url;
}

bool MongoDBConnectionPool::create_client_pool()
{
	mongoc_client_pool_t * client_pool = nullptr;
	mongoc_uri_t         *uri = nullptr;
	uri = mongoc_uri_new(m_strurl.c_str());//url指定ip地址，没有指定
	client_pool = mongoc_client_pool_new(uri);
	m_pool = static_cast<void*>(client_pool);
	m_url = static_cast<void*>(uri);
	return true;
}

bool MongoDBConnectionPool::destory_client_pool()
{
	if (m_url == nullptr || m_pool == nullptr)
	{
		return false;
	}
	mongoc_client_pool_destroy(static_cast<mongoc_client_pool_t*>(m_pool));
	mongoc_uri_destroy(static_cast<mongoc_uri_t*>(m_url));
	mongoc_cleanup();
	return true;
}

void MongoDBConnectionPool::free_bjson(const bson_t *command)
{
	char *commandStr = ::bson_as_json(command, NULL);
	bson_free(commandStr);
}

bool MongoDBConnectionPool::insert_coll(const char*databasename, const char* collname, const bson_t *doc)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;
	client = mongoc_client_pool_pop(pool);//从连接池中获取连接对象 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);
	// 将bson文档插入到集合
	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
		free_bjson(doc);
		mongoc_client_pool_push(pool, client);
		return false;
	}
	// 释放资源
	free_bjson(doc);
	mongoc_client_pool_push(pool, client);
	return true;
}


bool MongoDBConnectionPool::delete_coll(const char*databasename, const char* collname, const bson_t *doc)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;
	client = mongoc_client_pool_pop(pool);//从连接池中获取连接对象 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);
	// 执行删除操作。这里只能匹配_id字段，也就只能删除上面插入的文档
	if (!mongoc_collection_remove(collection, MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) {
		free_bjson(doc);
		mongoc_client_pool_push(pool, client);
		return false;
	}
	// 释放资源
	free_bjson(doc);
	mongoc_client_pool_push(pool, client);
	return true;
}


bool MongoDBConnectionPool::update_coll(const char*databasename, const char* collname, const bson_t* _id, const bson_t *doc)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;
	client = mongoc_client_pool_pop(pool);//从连接池中获取连接对象 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);
	// 执行update操作。这个操作将使用update的内容去替换之前插入到数据库中的doc的内容
	if (!mongoc_collection_update(collection, MONGOC_UPDATE_NONE, _id, doc, NULL, &error)) {
		free_bjson(_id);
		free_bjson(doc);
		mongoc_client_pool_push(pool, client);
		return false;
	}
	// 释放资源
	free_bjson(_id);
	free_bjson(doc);
	mongoc_client_pool_push(pool, client);
	return true;
}

bool MongoDBConnectionPool::select_coll(const char*databasename, const char* collname, bson_t* query, std::vector<std::map<std::string, std::string>>& msg_data)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;

	client = mongoc_client_pool_pop(pool);//从连接池中获取连接对象 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);

	// query是一个空的BSON文档，用于做查询说明符的时候匹配所有文档。
	// 执行查询操作
	mongoc_cursor_t *cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

	const bson_t *doc = nullptr;
	while (!mongoc_cursor_error(cursor, &error) &&
		mongoc_cursor_more(cursor)) {
		if (mongoc_cursor_next(cursor, &doc))
		{
			GetRecord(doc, msg_data);
		}
	}

	bool result = true;
	if (mongoc_cursor_error(cursor, &error))
		result = false;

	if (doc) free_bjson(doc);

	mongoc_cursor_destroy(cursor);
	mongoc_client_pool_push(pool, client);
	return result;
}


bool MongoDBConnectionPool::batch_insert_coll(const char*databasename, const char* collname, const bson_t **docs, size_t n_documents)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;
	client = mongoc_client_pool_pop(pool);//从连接池中获取连接对象 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}
	bson_t *reply = new bson_t;
	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);
	// 将bson文档插入到集合
	if (!mongoc_collection_insert_many(collection, docs, n_documents, NULL, NULL, &error)) {
		free_bjson(*docs);
		mongoc_client_pool_push(pool, client);
		return false;
	}
	// 释放资源
	free_bjson(*docs);
	mongoc_client_pool_push(pool, client);
	return true;
}

void MongoDBConnectionPool::GetRecord(const bson_t *doc, std::vector<std::map<std::string, std::string>>& msg_data)
{
	bson_iter_t iter;
	bson_iter_init(&iter, doc);
	uint32_t len = 12;
	const uint8_t* uintlen = nullptr;

	std::string key, value;
	bson_type_t type;
	std::map < std::string, std::string > tmp_data;
	tmp_data.clear();
	while (bson_iter_next(&iter))
	{
		key.clear(); value.clear();
		key = bson_iter_key(&iter);
		type = bson_iter_type(&iter);
		switch (type)
		{
		case BSON_TYPE_UTF8:
			value = bson_iter_utf8(&iter, &len);
			tmp_data[key] = value;
			break;
		case BSON_TYPE_ARRAY:
			bson_iter_array(&iter, &len, &uintlen);
			break;
		default:
			break;
		}

	}
	if (tmp_data.size() < 1) return;
	msg_data.push_back(tmp_data);
}


DBIOMongoDB& DBIOMongoDB::instance()
{
	static DBIOMongoDB s_instance;
	return s_instance;
}

bool DBIOMongoDB::mongodb_init(const char* url)
{
	MongoDBConnectionPool* client_pool = new MongoDBConnectionPool();
	client_pool->mongodb_init(url);
	client_pool->create_client_pool();

	m_mongodb_ptr = static_cast<void*>(client_pool);
	return true;
}

bool DBIOMongoDB::mongodb_cleanup()
{
	bool ulret = false;
	MongoDBConnectionPool* dbapi = static_cast<MongoDBConnectionPool*>(m_mongodb_ptr);
	if (dbapi == nullptr) { return ulret; }
	return dbapi->destory_client_pool();
}

bool DBIOMongoDB::select_coll(const char* dbname, const char* collname, const char* id_, std::vector<std::map<std::string, std::string>>& msg_data)
{
	MongoDBConnectionPool* dbapi = static_cast<MongoDBConnectionPool*>(m_mongodb_ptr);
	if (dbapi == nullptr) { return false; }

	//bson_t* cond = bson_new_from_json((const unsigned char*)command.c_str(), command.length(), &errorinfo);

	bson_t * query = bson_new();
	return dbapi->select_coll(dbname, collname, query, msg_data);
}


bool DBIOMongoDB::insert_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data)
{
	MongoDBConnectionPool* dbapi = static_cast<MongoDBConnectionPool*>(m_mongodb_ptr);
	if (dbapi == nullptr) { return false; }
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t* doc = bson_new();

	for (auto &itr : map_data)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(doc, key.c_str(), value.c_str());
	}
	return dbapi->insert_coll(dbname, collname, doc);
}




bool DBIOMongoDB::delete_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data)
{
	MongoDBConnectionPool* dbapi = static_cast<MongoDBConnectionPool*>(m_mongodb_ptr);
	if (dbapi == nullptr) { return false; }
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t* doc = bson_new();
	for (auto &itr : map_data)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(doc, key.c_str(), value.c_str());
	}
	return dbapi->delete_coll(dbname, collname, doc);
}


bool DBIOMongoDB::update_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_qurey, const std::map<std::string, std::string>& map_data)
{
	MongoDBConnectionPool* dbapi = static_cast<MongoDBConnectionPool*>(m_mongodb_ptr);
	if (dbapi == nullptr) { return false; }
	if (map_data.size() < 1 || map_qurey.size() < 1) return false;

	std::string key, value;
	bson_t* doc = bson_new();
	bson_t query;
	bson_init(&query);

	for (auto &itr : map_qurey)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(&query, key.c_str(), value.c_str());
	}
	for (auto &itr : map_data)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(doc, key.c_str(), value.c_str());
	}
	return dbapi->update_coll(dbname, collname, &query, doc);
}

bool DBIOMongoDB::select_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data, std::vector<std::map<std::string, std::string>> & msg_data)
{
	MongoDBConnectionPool* dbapi = static_cast<MongoDBConnectionPool*>(m_mongodb_ptr);
	if (dbapi == nullptr) { return false; }
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t * query = bson_new();

	for (auto &itr : map_data)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(query, key.c_str(), value.c_str());
	}
	return dbapi->select_coll(dbname, collname, query, msg_data);
}

bool DBIOMongoDB::InsertBatchColl(const char* dbname, const char* collname, const std::vector<std::map<std::string, std::string>>& map_data)
{
	MongoDBConnectionPool* dbapi = static_cast<MongoDBConnectionPool*>(m_mongodb_ptr);
	if (dbapi == nullptr) { return false; }
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t* doc = bson_new();
	bson_t** docs = nullptr;

	for (auto &itr : map_data)
	{
		for (auto& itr_data : itr)
		{
			key = itr_data.first;
			value = itr_data.second;
			BSON_APPEND_UTF8(doc, key.c_str(), value.c_str());
		}
	}
	return false;
	//return dbapi->batch_insert_coll(dbname, collname, &doc, 11);
}
