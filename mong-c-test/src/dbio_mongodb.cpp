/*
 * dbio_mysql.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: root
 */
#include "dbio_mongodb.h"

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
	create_client_pool();
}

bool MongoDBConnectionPool::create_client_pool()
{
	mongoc_client_pool_t * client_pool = nullptr;
	mongoc_uri_t         *uri = nullptr;
	uri = mongoc_uri_new(m_strurl.c_str());//urlָ��ip��ַ��û��ָ��
	client_pool = mongoc_client_pool_new(uri);
	//mongoc_client_pool_max_size(client_pool, 100);
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

	mongoc_client_pool_t* pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t* client;
	bson_error_t error;
	client = mongoc_client_pool_pop(pool);//�����ӳ��л�ȡ���Ӷ��� 
	if (client == nullptr) {
		return false;
	}

	mongoc_collection_t* collection = mongoc_client_get_collection(client, databasename, collname);
	// ��bson�ĵ����뵽����
	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
		free_bjson(doc);
		mongoc_client_pool_push(pool, client);
		return false;
	}
	// �ͷ���Դ
	free_bjson(doc);
	mongoc_client_pool_push(pool, client);
	return true;
}


bool MongoDBConnectionPool::delete_coll(const char*databasename, const char* collname, const bson_t *doc)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;
	client = mongoc_client_pool_pop(pool);//�����ӳ��л�ȡ���Ӷ��� 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);
	// ִ��ɾ������������ֻ��ƥ��_id�ֶΣ�Ҳ��ֻ��ɾ�����������ĵ�
	if (!mongoc_collection_remove(collection, MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) {
		free_bjson(doc);
		mongoc_client_pool_push(pool, client);
		return false;
	}
	// �ͷ���Դ
	free_bjson(doc);
	mongoc_client_pool_push(pool, client);
	return true;
}


bool MongoDBConnectionPool::update_coll(const char*databasename, const char* collname, const bson_t* _id, const bson_t *doc)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;
	client = mongoc_client_pool_pop(pool);//�����ӳ��л�ȡ���Ӷ��� 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);
	// ִ��update���������������ʹ��update������ȥ�滻֮ǰ���뵽���ݿ��е�doc������
	if (!mongoc_collection_update(collection, MONGOC_UPDATE_NONE, _id, doc, NULL, &error)) {
		free_bjson(_id);
		free_bjson(doc);
		mongoc_client_pool_push(pool, client);
		return false;
	}
	// �ͷ���Դ
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

	client = mongoc_client_pool_pop(pool);//�����ӳ��л�ȡ���Ӷ��� 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);

	// query��һ���յ�BSON�ĵ�����������ѯ˵������ʱ��ƥ�������ĵ���
	// ִ�в�ѯ����
	mongoc_cursor_t *cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

	const bson_t *doc;
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
	delete doc;

	mongoc_cursor_destroy(cursor);
	mongoc_client_pool_push(pool, client);
	return result;
}
bool MongoDBConnectionPool::select_coll_by_fields(const char*databasename, const char* collname, bson_t* query, bson_t* fields, std::vector<std::map<std::string, std::string>>& msg_data)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;

	client = mongoc_client_pool_pop(pool);//�����ӳ��л�ȡ���Ӷ��� 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);

	// query��һ���յ�BSON�ĵ�����������ѯ˵������ʱ��ƥ�������ĵ���
	// ִ�в�ѯ����
	mongoc_cursor_t *cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, fields, NULL);

	const bson_t *doc;
	while (!mongoc_cursor_error(cursor, &error) &&
		mongoc_cursor_more(cursor)) {
		if (mongoc_cursor_next(cursor, &doc))
		{
			GetVecRecord(doc, msg_data);
		}
	}

	bool result = true;
	if (mongoc_cursor_error(cursor, &error))
		result = false;

	if (doc) free_bjson(doc);
	delete doc;

	mongoc_cursor_destroy(cursor);
	mongoc_client_pool_push(pool, client);
	return result;
}

bool  MongoDBConnectionPool::find_coll(const char*databasename, const char* collname, bson_t* query)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;

	client = mongoc_client_pool_pop(pool);//�����ӳ��л�ȡ���Ӷ��� 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}

	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);

	// query��һ���յ�BSON�ĵ�����������ѯ˵������ʱ��ƥ�������ĵ���
	// ִ�в�ѯ����
	mongoc_cursor_t *cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

	const bson_t *doc;
	while (!mongoc_cursor_error(cursor, &error) &&
		mongoc_cursor_more(cursor)) {
		if (mongoc_cursor_next(cursor, &doc))
		{
			return true;
		}
	}
	mongoc_cursor_destroy(cursor);
	mongoc_client_pool_push(pool, client);
	return false;
}


bool MongoDBConnectionPool::batch_insert_coll(const char*databasename, const char* collname,const bson_t **docs, size_t n_documents)
{
	mongoc_client_pool_t *pool = static_cast<mongoc_client_pool_t*>(m_pool);
	mongoc_client_t      *client;
	bson_error_t error;
	client = mongoc_client_pool_pop(pool);//�����ӳ��л�ȡ���Ӷ��� 
	if (client == nullptr) {
		//H3C_CLOUD_MONGODB_LOG1(H3C_LOG_ERROR, "fail get client ptr url:[%s] database name:[%s]", databasename);
		return false;
	}
	bson_t *reply = new bson_t;
	mongoc_collection_t *collection = mongoc_client_get_collection(client, databasename, collname);
	// ��bson�ĵ����뵽����
	if (!mongoc_collection_insert_many(collection, docs, n_documents, NULL, NULL, &error)) {
		free_bjson(*docs);
		mongoc_client_pool_push(pool, client);
		return false;
	}
	// �ͷ���Դ
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

void MongoDBConnectionPool::GetVecRecord(const bson_t *doc, std::vector<std::map<std::string, std::string>>& msg_data)
{
	bson_iter_t iter;
	bson_iter_init(&iter, doc);
	uint32_t len = 12;
	const uint8_t* uintlen = nullptr;
	bson_iter_t child;
	bson_iter_t child1;

	std::string key, value, k1, value1;
	bson_type_t type;
	std::map < std::string, std::string > tmp_data;
	while (bson_iter_next(&iter))
	{
		key = bson_iter_key(&iter);
		type = bson_iter_type(&iter);
		switch (type)
		{
		case BSON_TYPE_UTF8:
			value = bson_iter_utf8(&iter, &len);
			tmp_data[key] = value;
			break;
		case BSON_TYPE_ARRAY:
			tmp_data.clear();
			bson_iter_array(&iter, &len, &uintlen);
			bson_iter_recurse(&iter, &child);
			while (bson_iter_next(&child)) {
				bson_iter_recurse(&child, &child1);
				while (bson_iter_next(&child1)) {
					k1 = bson_iter_key(&child1);
					type = bson_iter_type(&child1);
					if (BSON_TYPE_UTF8 == type)
						value = bson_iter_utf8(&child1, &len);
					tmp_data[k1] = value;
				}
				msg_data.push_back(tmp_data);
			}
			break;
		default:
			break;
		}

	}
	if (tmp_data.size() < 1) return;
	//msg_data.push_back(tmp_data);
}





DBIOMongoDB& DBIOMongoDB::instance()
{
	static DBIOMongoDB s_instance;
	return s_instance;
}

DBIOMongoDB::DBIOMongoDB():m_mongodb_ptr(NULL)
{

}

DBIOMongoDB::~DBIOMongoDB()
{

}

bool DBIOMongoDB::mongodb_init(const char* url)
{
	m_mongodb_ptr.reset(new MongoDBConnectionPool());
	m_mongodb_ptr->mongodb_init(url);
	return true;
}

bool DBIOMongoDB::mongodb_cleanup()
{
	return m_mongodb_ptr->destory_client_pool();
}

bool DBIOMongoDB::select_coll(const char* dbname, const char* collname, const char* id_, std::vector<std::map<std::string, std::string>>& msg_data)
{
	//bson_t* cond = bson_new_from_json((const unsigned char*)command.c_str(), command.length(), &errorinfo);

	bson_t * query = bson_new();
	return m_mongodb_ptr->select_coll(dbname, collname, query, msg_data);
}


bool DBIOMongoDB::insert_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data)
{
	std::string key, value;
	bson_t* doc = bson_new();

	for (auto &itr : map_data)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(doc, key.c_str(), value.c_str());
	}
	return m_mongodb_ptr->insert_coll(dbname, collname, doc);
}




bool DBIOMongoDB::delete_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data)
{
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t* doc = bson_new();
	for (auto &itr : map_data)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(doc, key.c_str(), value.c_str());
	}
	return m_mongodb_ptr->delete_coll(dbname, collname, doc);
}


bool DBIOMongoDB::update_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_qurey, const std::map<std::string, std::string>& map_data)
{
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
	return m_mongodb_ptr->update_coll(dbname, collname, &query, doc);
}

bool DBIOMongoDB::select_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data, std::vector<std::map<std::string, std::string>> & msg_data)
{
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t * query = bson_new();

	for (auto &itr : map_data)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(query, key.c_str(), value.c_str());
	}
	return m_mongodb_ptr->select_coll(dbname, collname, query, msg_data);
}

bool DBIOMongoDB::find_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data)
{
	std::string key, value;
	bson_t * query = bson_new();

	for (auto &itr : map_data)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(query, key.c_str(), value.c_str());
	}
	return m_mongodb_ptr->find_coll(dbname, collname, query);
}

bool DBIOMongoDB::InsertNoticeColl(const char* dbname, const char* collname, const std::map<std::string, std::string> id_map, const std::map<std::string, std::vector<std::map<std::string, std::string>>>& map_data)
{
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t* doc = bson_new();
	bson_t child;
	bson_t child2;
	char        buf[16];
	std::size_t      keylen;
	const       char *key1;
	bson_init(&child2);


	for (auto &itr : id_map)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(doc, key.c_str(), value.c_str());
	}
	//
	int i = 0;
	for (auto &itr : map_data)
	{
		key = itr.first;
		i = 0;
		BSON_APPEND_ARRAY_BEGIN(doc, key.c_str(), &child);
		for (auto &itr_child_vec: itr.second)
		{	
			keylen = bson_uint32_to_string(i, &key1, buf, sizeof buf);
			bson_append_document_begin(&child, key1, keylen, &child2);
			for (auto & itr_child_map : itr_child_vec)
			{
				key = itr_child_map.first;
				value = itr_child_map.second;
				BSON_APPEND_UTF8(&child2, key.c_str(), value.c_str());
			}
			i++;
			bson_append_document_end(&child, &child2);
		}
		bson_append_array_end(doc, &child);
	}

	return m_mongodb_ptr->insert_coll(dbname, collname, doc);
}


bool DBIOMongoDB::PushNoticeColl(const char* dbname, const char* collname, const std::map<std::string, std::string> id_map, const std::map<std::string, std::map<std::string, std::string>>& map_data)
{
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t* doc = bson_new();
	bson_t* query = bson_new();
	bson_t child;

	for (auto &itr : id_map)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(query, key.c_str(), value.c_str());
	}
	for (auto &itr : map_data)
	{
		key = itr.first;
		BSON_APPEND_DOCUMENT_BEGIN(doc, key.c_str(), &child);
		for (auto & itr_child_map : itr.second)
		{
			key = itr_child_map.first;
			value = itr_child_map.second;
			BSON_APPEND_UTF8(&child, key.c_str(), value.c_str());
		}
		bson_append_document_end(doc, &child);
	}
	bson_t* docs = bson_new();
	BSON_APPEND_DOCUMENT(docs, "$push", doc);

	return m_mongodb_ptr->update_coll(dbname, collname,query, docs);
}

bool DBIOMongoDB::DelNoticeColl(const char* dbname, const char* collname, const std::map<std::string, std::string> id_map, std::map<std::string, std::map<std::string, std::string>>& map_data)
{
	if (map_data.size() < 1) return false;

	std::string key, value;
	bson_t* doc = bson_new();
	bson_t* query = bson_new();
	bson_t child;


	for (auto &itr : id_map)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(query, key.c_str(), value.c_str());
	}
	for (auto &itr : map_data)
	{
		key = itr.first;
		BSON_APPEND_DOCUMENT_BEGIN(doc, key.c_str(), &child);
		for (auto & itr_child_map : itr.second)
		{
			key = itr_child_map.first;
			value = itr_child_map.second;
			BSON_APPEND_UTF8(&child, key.c_str(), value.c_str());
		}
		bson_append_document_end(doc, &child);
	}
	bson_t* docs = bson_new();
	BSON_APPEND_DOCUMENT(docs, "$pull", doc);

	return m_mongodb_ptr->update_coll(dbname, collname, query, docs);
}

bool DBIOMongoDB::SelectNoticeColl(const char* dbname, const char* collname, const std::map<std::string, std::string> id_map, const std::vector< std::string> field_map, std::vector<std::map<std::string, std::string>>& map_data)
{
	bson_t query;
	bson_t fields;
	std::string key, value;
	bson_init(&query);
	bson_init(&fields);

	for (auto& itr : id_map)
	{
		key = itr.first;
		value = itr.second;
		BSON_APPEND_UTF8(&query, key.c_str(), value.c_str());
	}
	for (auto& itr : field_map)
	{
		key = itr;
		BSON_APPEND_UTF8(&fields, key.c_str(), "1");
	}

	return m_mongodb_ptr->select_coll_by_fields(dbname, collname,&query, &fields, map_data);
}