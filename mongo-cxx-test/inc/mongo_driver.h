#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <thread>

// 官方文档中缺少以下语句
#include <bsoncxx\builder\stream\document.hpp>       

#include <windows.h>


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;



int test() 
{
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::uri uri("mongodb://192.168.0.130:30010");
    mongocxx::client client(uri);
    mongocxx::database mydb = client["notice_set"];
    mongocxx::collection mycoll = mydb["test"];

    mongocxx::collection mycoll1 = mydb["test1"];
    
    mongocxx::collection mycoll2 = mydb["test2"];

    for (int i=1; i<=100000000; ++i)
    {
        auto builder = bsoncxx::builder::stream::document{};
        builder << "msg_id" << "AE216416-35D5-11EA-AC7C-408D5CC95D15"
            << "runboot" << "content-length: 78 \
			content - type: Text / Btf\
			msgflag : 0\
			msgtype : 0\
			receive - users : 1105; user90105\
			senddate : 1578900246501172\
			senderid : 1104\
			sendername : user90104\
			senderssid : 024A5517 - 7AA5 - B123 - 7A15 - 7FEC34CE1D6C\
			subject : qwe\
\
			<BTF><Font name = '宋体' size = '15' clr = '0' flags = '1' / ><Text>qwe< / Text>< / BTF>";

        auto result = mycoll.insert_one(builder.view());
        auto result1 = mycoll1.insert_one(builder.view());
        auto result2 = mycoll2.insert_one(builder.view());
        bsoncxx::oid oid = result->inserted_id().get_oid().value;

        std::cout << "insert one: " << i << " _id : " << result->inserted_id().get_oid().value.to_string() << "\n";
    }
    return 1;

}

int testPool()
{
    int sumnumber = 0;
    int num =  0;
    std::string m("mongodb://192.168.0.130:30010");
    mongocxx::instance instance{};
    mongocxx::uri uri(m.c_str());
    mongocxx::pool pool{uri};
    printf("mongo地址：mongodb://192.168.0.130:30010\n");

    auto threadFunc = [&num, &sumnumber](const mongocxx::client& client,const std::string& dbname) {
        for (int i = 0; i < 100000; i ++) {
            auto builder = bsoncxx::builder::stream::document{};
            builder << "msg_id" << "AE216416-35D5-11EA-AC7C-408D5CC95D15"
                << "msg_data" << "content-length: 78 \
                content - type: Text / Btf\
                msgflag : 0\
                msgtype : 0\
                receive - users : 1105; user90105\
                senddate : 1578900246501172\
                senderid : 1104\
                sendername : user90104\
                senderssid : 024A5517 - 7AA5 - B123 - 7A15 - 7FEC34CE1D6C\
                subject : qwe\
                \
                <BTF><Font name = '宋体' size = '15' clr = '0' flags = '1' / ><Text>qwe< / Text>< / BTF>";

            auto col = client[dbname]["notice"].insert_one(builder.view());
            auto col1 = client[dbname]["202004"].insert_one(builder.view());
            auto col2 = client[dbname]["notice_mob"].insert_one(builder.view());
            num += 3;
            sumnumber += 3;
        }
        
    };

    for (int b = 0; b = 150; b ++) {
        std::thread t1( [&] () {
            auto c = pool.acquire();
            threadFunc(*c, "notice_set");
            threadFunc(*c, "notice_set");
        });
        t1.detach();
    }

    std::thread t2( [=, &num]() {
        while (true){
            Sleep(1);
        }
    } );
    t2.join();

    return 1;
}