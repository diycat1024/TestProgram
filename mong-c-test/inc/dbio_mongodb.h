#pragma once
#include <string>

#include <vector>
#include <map>
#include <bson.h>
#include <mongoc.h>


using namespace std;


class MongoDBConnectionPool
{
public:
	MongoDBConnectionPool();
	~MongoDBConnectionPool();

public:
	void mongodb_init(const char* url);

	bool create_client_pool();

	bool destory_client_pool();

	void free_bjson(const bson_t *command);
	//mongodb��һϵ�е����е�����������������������Ϣ�����
	bool  insert_coll(const char*databasename, const char* collname, const bson_t *doc);
	bool  delete_coll(const char*databasename, const char* collname, const bson_t *doc);
	bool  update_coll(const char*databasename, const char* collname, const bson_t* _id, const bson_t *doc);
	bool  select_coll(const char*databasename, const char* collname, bson_t* query, std::vector<std::map<std::string, std::string>>& doc);

	bool batch_insert_coll(const char*databasename, const char* collname, const  bson_t **docs, size_t n_documents);

	void GetRecord(const bson_t *doc, std::vector<std::map<std::string, std::string>>& msg_data);
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
	/*MongoDB���ݿ��ʼ��*/
	bool mongodb_init(const char* url);
	/*�������ݿ�*/
	bool mongodb_cleanup();

	bool select_coll(const char* dbname, const char* collname, const char* id_, std::vector<std::map<std::string, std::string>>& msg_data);

	bool insert_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data);
	bool delete_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data);
	bool update_coll(const char* dbname, const char* collname,
		const std::map<std::string, std::string>& map_qurey,
		const std::map<std::string, std::string>& map_data);
	bool select_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data, std::vector<std::map<std::string, std::string>> & msg_data);
	bool InsertBatchColl(const char* dbname, const char* collname, const std::vector<std::map<std::string, std::string>>& map_data);
private:
	void* m_mongodb_ptr = nullptr;
};