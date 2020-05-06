#include "dbio_mongodb.h"

DBIOMongoDB& DBIOMongoDB::instance()
{
    static DBIOMongoDB g_in;
    return g_in;
}
DBIOMongoDB::DBIOMongoDB():pool_(nullptr)
{
}

DBIOMongoDB::~DBIOMongoDB()
{

}

bool DBIOMongoDB::mongodb_init(const char* url)
{
    pool_.reset (new mongocxx::pool(mongocxx::uri(url)));
    //pool_ = static_cast<void*>(pool);
    return true;    
}
bool DBIOMongoDB::mongodb_cleanup()
{
    return true;
}

bool DBIOMongoDB::insert_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data)
{

    auto builder =bsoncxx::builder::stream::document{};
    for (auto & iter : map_data) 
    {
        builder << iter.first << iter.second;
    }
    builder << close_document;
    builder << finalize;

    auto client = pool_->acquire();
    (*client)[dbname][collname].insert_one(builder.view()); 
    return true;
}

bool DBIOMongoDB::delete_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data)
{
    auto builder =bsoncxx::builder::stream::document{};
    for (auto & iter : map_data) 
    {
        builder << iter.first << iter.second;
    }
    builder << finalize;

    auto client = pool_->acquire();
    (*client)[dbname][collname].delete_one(builder.view()); 
    return true;
}

bool DBIOMongoDB::update_coll(const char* dbname, const char* collname,
    const std::map<std::string, std::string>& map_qurey,
    const std::map<std::string, std::string>& map_data)
{
    auto query =bsoncxx::builder::stream::document{};
    auto builder =bsoncxx::builder::stream::document{};
    for (auto &iter: map_qurey)
    {
        query<< iter.first << iter.second;
    }
    query<< finalize;
    for (auto &iter : map_data) 
    {
        builder << iter.first << iter.second;
    }
    builder << finalize;

    auto client = pool_->acquire();
    (*client)[dbname][collname].update_one(query.view(), builder.view()); 
    return true;
}

//bool DBIOMongoDB::select_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data, std::vector<std::map<std::string, std::string>>& msg_data)
//{
//	auto builder = bsoncxx::builder::stream::document{};
//	for (auto& iter : map_data)
//	{
//		builder << iter.first << iter.second;
//	}
//	//builder << finalize;
//
//
//	auto client = pool_->acquire();
//	mongocxx::cursor cursor = (*client)[dbname][collname].find(builder.view());
//
//	std::map<std::string, std::string> msg;
//	for (const bsoncxx::document::view& docView : cursor)
//	{
//		std::string str = bsoncxx::to_json(docView);
//		std::vector<std::string> doc_keys;
//		std::transform(std::begin(docView), std::end(docView), std::back_inserter(doc_keys), [](bsoncxx::document::element ele)
//		{
//			return ele.key().to_string();
//		});
//
//		for (auto key : doc_keys) {
//			if (key == "_id")continue;
//
//			bsoncxx::document::element elem;
//			elem = docView[key];
//			if (elem.type() == bsoncxx::type::k_utf8)
//			{
//				msg[key] = elem.get_utf8().value.to_string();
//			}
//			else {
//
//			}
//		}
//		msg_data.push_back(msg);
//	}
//
//	return true;
//}


bool DBIOMongoDB::select_coll(const char* dbname, const char* collname, const std::map<std::string, std::string>& map_data, std::vector<std::map<std::string, std::string>> & msg_data)
{
    auto builder =bsoncxx::builder::stream::document{};
    for (auto & iter : map_data) 
    {
        builder << iter.first << iter.second;
    }
    //builder << finalize;

    
    auto client = pool_->acquire();
    mongocxx::cursor cursor = (*client)[dbname][collname].find(builder.view()); 

    std::map<std::string, std::string> msg;
    for (const bsoncxx::document::view &docView : cursor)
    {
        for (bsoncxx::document::element d : docView)
        {
            std::string k = d.key().to_string();
            if (k.empty()) continue;
            if (k == "_id") continue;
            
			if (d.type() == bsoncxx::type::k_utf8)
			{
				msg[k] = d.get_utf8().value.to_string();
			}
			else {

			}
        }
        msg_data.push_back(msg);
    }

    return true;
}

bool DBIOMongoDB::InsertBatchColl(const char* dbname, const char* collname, const std::vector<std::map<std::string, std::string>>& map_data)
{
    return true;
}



