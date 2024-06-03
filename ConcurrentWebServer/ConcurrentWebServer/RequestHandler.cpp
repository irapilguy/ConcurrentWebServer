#include "RequestHandler.h"

#include <stdexcept>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library


void RequestHandler::handleRequest(SOCKET clientSocket) {

    try {
        std::string request = parseRequest(clientSocket);
        std::cout << request << "\n";

        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
            "<html><body><h1>Hello, World!</h1></body></html>";
        sendResponse(clientSocket, response);
    }
    catch (std::runtime_error& error) {
        std::cerr << error.what() << "\n";
    }
}

std::string RequestHandler::parseRequest(SOCKET clientSocket) {

    const int bufferSize = 1024;
    char buffer[bufferSize];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        throw std::runtime_error("recv() failed");
    }
    else if (bytesReceived == 0) {
        throw std::runtime_error("Client disconnected");
    }
   
    return std::string(buffer, bytesReceived);
}

void RequestHandler::sendResponse(SOCKET clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), static_cast<int>(response.length()), 0);
}