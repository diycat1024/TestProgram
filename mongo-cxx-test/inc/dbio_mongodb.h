#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/options/find.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx\builder\stream\document.hpp> 
#include <bsoncxx/stdx/string_view.hpp>   
#include <bsoncxx/json.hpp>   

#include <memory>
#include <map>
#include <cstdint>
#include <iostream>
#include <vector>
#include "acl/lib_acl.h"
#include "acl_cpp/lib_acl.hpp"

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::type;
using namespace bsoncxx;

class  DBIOMongoDB
{
public:
	static DBIOMongoDB& instance();
    DBIOMongoDB();
    ~DBIOMongoDB();
	bool mongodb_init(const char* url);
	bool mongodb_cleanup();


	bool insert_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data);
	bool delete_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data);
	bool update_coll(const char* dbname, const char* collname,
		const std::map<std::string, std::string>& map_qurey,
		const std::map<std::string, std::string>& map_data);
	bool select_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data, std::vector<std::map<std::string, std::string>> & msg_data);
	bool InsertBatchColl(const char* dbname, const char* collname, const std::vector<std::map<std::string, std::string>>& map_data);
private:
    std::shared_ptr<mongocxx::pool> pool_;
};