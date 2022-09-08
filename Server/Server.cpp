#include "Server.h"
#include <sstream>
#include<Windows.h>

Server::Server(string ipAdress, int port)
{
    server_ip_address = ipAdress;
    server_port = port;
    temps();
}
Server::~Server() {
    cleanup();
}

// send message to client
void Server::Send(int clientSocket, string msg) {
    send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

void Server::temps()
{
    cout << "Enter Temperatures Degrees count." << endl;
    int count;
    cin >> count;
    for (int i = 0; i < count; i++) {
        cout << "Enter Temperature Grade in Celsius" << endl;
        float degree;
        cin >> degree;
        temperatures.push_back(degree);
    }
}

//initialize Winsock
bool Server::Init() {
    WSADATA data;
    WORD ver = MAKEWORD(2, 2);

    int wsInit = WSAStartup(ver, &data);
    //Inform client that error occurred

    return wsInit == 0;
}

//the main loop
void Server::run()
{

    while (true)
    {
        //create listening socket
        SOCKET listening = createSocket();
        if (listening == INVALID_SOCKET) {
            break;
        }

        SOCKET client = waitForConnection(listening);

        if (client != INVALID_SOCKET)
        {
            cout << "Client connected";
            closesocket(listening);
            int i = 0;
            int milli_seconds = 1000;
            while (i < temperatures.size()) {
                stringstream ss;
                float k = temperatures[i];
                ss << k;
                string grade;
                ss >> grade;
                Send(client, grade);

                Sleep(milli_seconds);
            }
            Send(client, "Exit");
            closesocket(client);
        }
    }
}

//clean up
void Server::cleanup() {
    WSACleanup();
}

//create socket
SOCKET Server::createSocket() {
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening != INVALID_SOCKET) {
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(server_port);
        inet_pton(AF_INET, server_ip_address.c_str(), &hint.sin_addr);

        int bindOK = bind(listening, (sockaddr*)&hint, sizeof(hint));
        if (bindOK != SOCKET_ERROR) {
            int listenOK = listen(listening, SOMAXCONN);
            if (listenOK == SOCKET_ERROR) {
                return -1;
            }
        }
        else {
            return -1;
        }
        return listening;
    }
}

string Server::get_ipAdress() { return server_ip_address; }
int Server::get_port() {
    return server_port;
}

//wait for connection
SOCKET Server::waitForConnection(SOCKET listening)
{
    SOCKET client = accept(listening, NULL, NULL);
    return client;
}