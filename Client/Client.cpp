#include "Client.h"
#include <chrono>

Client::Client(string ipAddress, int port_num) {
	ip_address = ipAddress;
	port = port_num;
	avg_over_time = 0;
	accum_over_time = 0;
}
Client::~Client() {
	cleanup();
}

bool Client::Init() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);


	return wsResult == 0;
}

SOCKET Client::createClientSocket() {
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		cleanup();
		return -1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ip_address.c_str(), &hint.sin_addr);

	int connection_result = connect(client_socket, (sockaddr*)&hint, sizeof(hint));
	if (connection_result == SOCKET_ERROR) {
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(client_socket);
		cleanup();
		return -1;
	}
	return client_socket;
}


void Client::run() {

	string msg;
	char buf[5000];
	auto start = chrono::steady_clock::now();

	SOCKET client_socket = createClientSocket();

	while (true) {
		ZeroMemory(buf, 5000);
		int bytesReceived = recv(client_socket, buf, 5000, 0);
		if (bytesReceived > 0) {
			msg = string(buf, 0, bytesReceived);
			if (msg == "Exit") {
				break;
			}
			float degree = stof(msg);
			temps.push_back(degree);
			calc_accum_over_time(degree);
			calc_avg_over_time();
		}
		auto end = chrono::steady_clock::now();
		if (chrono::duration_cast <chrono::seconds> (end - start).count() == 5) {
			cout << "Average Over Time is " << avg_over_time << endl;
			cout << "Accumulation Over Time is" << accum_over_time << endl;
			start = chrono::steady_clock::now();
		}

	}

	closesocket(client_socket);
	cleanup();


}

void Client::cleanup() {
	WSACleanup();
}

void Client::calc_avg_over_time() {
	avg_over_time = accum_over_time / temps.size();
}

void Client::calc_accum_over_time(float degree) {
	accum_over_time = accum_over_time + degree;

}
vector<float> Client::getTemps() {
	return temps;
}
float Client::get_agv() {
	return avg_over_time;
}
float Client::get_accum() { return accum_over_time; }
string Client::get_ipAdress() { return ip_address; }
int Client::get_port() {
	return port;
}

int main() {
	Client client = Client("127.0.0.1", 8080);

	if (client.Init()) {
		client.run();
	}

	return 0;
}