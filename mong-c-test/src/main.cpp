#include <iostream>
#include "dbio_mongodb.h"
#include "ThreadPool.h"

void insertMongoDB()
{
	DBIOMongoDB api = DBIOMongoDB::instance();
	
}

int main()
{
	DBIOMongoDB api = DBIOMongoDB::instance();
	api.mongodb_init("mongodb://192.168.0.224:27017/");

	ThreadPool pool(100);

	// enqueue and store future
	auto result = pool.enqueue([](DBIOMongoDB& answer) {

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
	}, api);
	auto threadFun = [](DBIOMongoDB& answer) {

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

			std::thread::id tid = std::this_thread::get_id();
			std::cout << " current tid:" << tid << std::endl;
		}
	};
	for (int i = 0; i < 150; i++)
	{
		std::thread t1(threadFun, api);
		t1.detach();
	}
	while (true)
	{
		Sleep(10);
	}

	system("pause");
	return 0;
}