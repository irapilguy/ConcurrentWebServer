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
    
    ThreadPool threadPool;
    RequestHandler requestHandler;

    void acceptConections();
    void handleClient(SOCKET clientSocket);
};