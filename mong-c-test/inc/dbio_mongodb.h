
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <bson/bson.h>
#include <mongoc/mongoc.h>

using namespace std;

class  MongoDBConnectionPool
{
public:
	MongoDBConnectionPool();
	~MongoDBConnectionPool();

public:
	void mongodb_init(const char* url);

	bool create_client_pool();

	bool destory_client_pool();

	void free_bjson(const bson_t *command);
	bool  insert_coll(const char*databasename, const char* collname, const bson_t *doc);
	bool  delete_coll(const char*databasename, const char* collname, const bson_t *doc);
	bool  update_coll(const char*databasename, const char* collname, const bson_t* _id, const bson_t *doc);
	bool  select_coll(const char*databasename, const char* collname, bson_t* query, std::vector<std::map<std::string, std::string>>& doc);
	bool  select_coll_by_fields(const char*databasename, const char* collname, bson_t* query, bson_t* fields, std::vector<std::map<std::string, std::string>>& msg_data);
	bool  find_coll(const char*databasename, const char* collname, bson_t* query);

	bool batch_insert_coll(const char*databasename, const char* collname, const  bson_t **docs, size_t n_documents);

	void GetRecord(const bson_t *doc, std::vector<std::map<std::string, std::string>>& msg_data);
	void GetVecRecord(const bson_t *doc, std::vector<std::map<std::string, std::string>>& msg_data);
private:
	int m_ulthreads = 10;
	void *  m_pool = nullptr;
	void *  m_url = nullptr;
	std::string  m_strurl;
};


class  DBIOMongoDB
{
public:
	static DBIOMongoDB& instance();

	DBIOMongoDB();
	virtual ~DBIOMongoDB();
	bool mongodb_init(const char* url);
	bool mongodb_cleanup();
	
	bool select_coll(const char* dbname, const char* collname, const char* id_, std::vector<std::map<std::string, std::string>>& msg_data);

	bool insert_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data);
	bool delete_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data);
	bool update_coll(const char* dbname, const char* collname, 
		const std::map<std::string, std::string>& map_qurey, 
		const std::map<std::string, std::string>& map_data);
	bool select_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data, std::vector<std::map<std::string, std::string>> & msg_data);

	bool  find_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data);

	bool InsertNoticeColl(const char* dbname, const char* collname, const std::map<std::string, std::string> id_map, const std::map<std::string, std::vector<std::map<std::string, std::string>>>& map_data);
	bool PushNoticeColl(const char* dbname, const char* collname, const std::map<std::string, std::string> id_map, const std::map<std::string, std::map<std::string, std::string>>& map_data);
	bool DelNoticeColl(const char* dbname, const char* collname, const std::map<std::string, std::string> id_map, std::map<std::string,std::map<std::string, std::string>>& map_data);
	bool SelectNoticeColl(const char* dbname, const char* collname, const std::map<std::string, std::string> id_map, const std::vector< std::string> field_map, std::vector<std::map<std::string, std::string>>& map_data);
private:
	std::shared_ptr<MongoDBConnectionPool> m_mongodb_ptr;
};