#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

class Client
{
private:
	string ip_address;
	int port;

	SOCKET createClientSocket();
	vector<float> temps;
	float avg_over_time;
	float accum_over_time;

public:

	Client(string ip_address, int port);
	~Client();

	bool Init();

	void run();
	void cleanup();

	void calc_avg_over_time();
	void calc_accum_over_time(float degree);

};