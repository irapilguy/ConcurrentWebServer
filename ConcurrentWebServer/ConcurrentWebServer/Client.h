#pragma once

#include <string>

typedef unsigned long long SOCKET;

class Client {
public:
    Client(const std::string& serverAddress, int port);
    ~Client();
    Client(const Client& other);

    bool connectToServer();
    std::string sendRequest(const std::string& request);

private:
    std::string serverAddress;
    int port;
    SOCKET connectSocket;
};