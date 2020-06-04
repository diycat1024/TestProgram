#include "dbio_mongodb.h"
#include <thread>

int main() 
{
    mongocxx::instance instance{};
    DBIOMongoDB dbio(DBIOMongoDB::instance());
    dbio.mongodb_init("mongodb://192.168.0.224:27017/?minPoolSize=4");
    std::map<std::string, std::string> map_data;
    std::vector<std::map<std::string, std::string>> msg_data;

    auto threadFunc =  [&](DBIOMongoDB& answer) {

		std::map<std::string, std::string >msg_data;
		msg_data["msg_id"] = "AE216416-35D5-11EA-AC7C-408D5CC95D15";
		msg_data["runboot"] = "content-length: 78 \
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
			<BTF><Font name = '����' size = '15' clr = '0' flags = '1' / ><Text>qwe< / Text>< / BTF>";
		for (int i = 0; i < 200000; i++)
		{
			msg_data["id"] = std::to_string(i);
			answer.insert_coll("notice111", "sites_1411", msg_data);

		}
	};

for (int b= 0; b < 150; b ++) {
	std::thread t1(threadFunc, dbio);
	t1.detach();
	}

	std::thread t2( [&]() {
		while(true)
		{
			Sleep(1000);
		}
	});
	t2.join();
    return 1;
}