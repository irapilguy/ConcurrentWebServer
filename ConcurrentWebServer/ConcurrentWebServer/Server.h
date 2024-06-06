#pragma once

#include <string>

#include "ThreadPool.h"
#include "RequestHandler.h"

typedef unsigned long long SOCKET;

class Server {
public:
    Server(const std::string& address, int post, int threadsCount);
    ~Server();

    Server(const Server& other);

    void start();
    void stop();

private:
    std::string address;
    int port;
    SOCKET serverSocket;
    int threadsCount;

    std::mutex acceptMutex;
    
    ThreadPool threadPool;
    RequestHandler requestHandler;

    volatile bool serverRunning;

    void acceptConections();
    void handleClient(SOCKET clientSocket);
};