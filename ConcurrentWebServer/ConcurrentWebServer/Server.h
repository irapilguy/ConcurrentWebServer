#pragma once
#include <winsock2.h>
#include <string>
#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

#include "ThreadPool.h"

class Server {
public:
    Server(const std::string& address, int post, int threadsCount);

    void start();
    void stop();

private:
    std::string address;
    int port;
    SOCKET serverSocket;
    int threadsCount;

    ThreadPool threadPool;
    void acceptConections();
    void handleClient(SOCKET clientSocket);
};