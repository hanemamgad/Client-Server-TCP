#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ws2tcpip.h> // header file for winsock functions

#pragma comment(lib, "ws2_32.lib") //winsock library file


using namespace std;
// for declaration
class Server;

typedef void(*MessageRecievedHandler)(Server* sv, int socket_id, string msg);

class Server
{
private:
    //create socket
    SOCKET createSocket();

    //wait for connection
    SOCKET waitForConnection(SOCKET listening);

    string server_ip_address;
    int server_port;
    MessageRecievedHandler msg_recieved;


public:
    vector<float> temperatures;
    Server(string ipAdress, int port);
    ~Server();

    // send message to client
    void Send(int clientSocket, string msg);

    //initialize Winsock
    bool Init();

    //the main loop
    void run();

    //clean up
    void cleanup();

    //fill temperatures
    void temps();
};