#include "dbio_mongodb.h"
int main() 
{
    mongocxx::instance instance{};
    DBIOMongoDB dbio(DBIOMongoDB::instance());
    dbio.mongodb_init("mongodb://192.168.0.212:27017/?minPoolSize=3");
    std::map<std::string, std::string> map_data;
    std::vector<std::map<std::string, std::string>> msg_data;

    map_data["msg_id"] = "37DF51AE-7E34-11EA-B761-74D435DDADF7";
    bool ret = dbio.select_coll("notice_set", "notice",map_data ,msg_data);

    return 1;
}