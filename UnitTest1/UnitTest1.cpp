#include "pch.h"
#include "CppUnitTest.h"
#include "../Client/Client.h"
#include "../Server/Server.h"
#include <vector>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestClassClient)
		{
			int port_num = 8080;
			string ip_address = "127.0.0.1";
			Client client = Client(ip_address, port_num);

			Assert::AreEqual(client.get_ipAdress(), ip_address);
			Assert::AreEqual(client.get_port(), port_num);

		}
		TEST_METHOD(TestClassServer)
		{
			int port_num = 8080;
			string ip_address = "127.0.0.1";
			Server server = Server(ip_address, port_num);

			Assert::AreEqual(server.get_ipAdress(), ip_address);
			Assert::AreEqual(server.get_port(), port_num);

		}
		TEST_METHOD(TestInitServer)
		{
			int port_num = 8080;
			string ip_address = "127.0.0.1";
			Server server = Server(ip_address, port_num);
			bool in = server.Init();
			Assert::AreEqual(in, true);
		}
		TEST_METHOD(TestInitClient)
		{
			int port_num = 8080;
			string ip_address = "127.0.0.1";
			Client client = Client(ip_address, port_num);
			bool in = client.Init();
			Assert::AreEqual(in, true);
		}
		TEST_METHOD(TestCalcAvgClient)
		{
			int port_num = 8080;
			string ip_address = "127.0.0.1";
			Client client = Client(ip_address, port_num);
			vector<float> temperatures;
			temperatures.push_back(30);
			temperatures.push_back(36.7);
			temperatures.push_back(23.8);
			temperatures.push_back(28.9);
			temperatures.push_back(33.45);
			float sum = 0;

			client.temps = temperatures;

			for (int i = 0; i < temperatures.size(); i++) {
				sum += temperatures[i];
				client.calc_accum_over_time(temperatures[i]);
			}
			float avg = sum / temperatures.size();

			client.calc_avg_over_time();
			Assert::AreEqual(avg, client.get_agv());

		}
	};
}
