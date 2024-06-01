#pragma once
#include <winsock2.h>
#include <string>
#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

class Server {
public:
    Server(const std::string& address, int post);

    void start();
    void stop();

private:
    std::string address;
    int port;
    SOCKET serverSocket;

    void acceptConections();
    void handleClient(SOCKET clientSocket);
};