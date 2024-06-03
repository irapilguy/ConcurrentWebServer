#pragma once

#include <string>

typedef unsigned long long SOCKET;


class RequestHandler {
public:
    void handleRequest(SOCKET clientSocket);
private:
    std::string parseRequest(SOCKET clientSocket);
    void sendResponse(SOCKET clientSocket, const std::string& response);
};