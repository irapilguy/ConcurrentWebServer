#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <iostream>

Server::Server(const std::string& address, int port, int threadsCount)
    : address(address)
    , port(port)
    , threadsCount(threadsCount)
    , threadPool(threadsCount)
{
}

void Server::start() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket\n";
        WSACleanup();
        return;
    }

    // Bind socket
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(address.c_str());
    serverAddr.sin_port = htons(port);
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server started, listening on port " << port << std::endl;

    acceptConections();
}

void Server::acceptConections() {
    // Accept incoming connections
    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed\n";
            closesocket(serverSocket);
            WSACleanup();
            return;
        }

        // Handle client
        threadPool.enqueue([this, clientSocket] {handleClient(clientSocket); });
    }
}

void Server::handleClient(SOCKET clientSocket) {
    const int bufferSize = 1024;
    char buffer[bufferSize];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "recv() failed\n";
    }
    else if (bytesReceived == 0) {
        std::cout << "Client disconnected\n";
    }
    else {
        std::cout << "Received message from client: " << std::string(buffer, bytesReceived) << std::endl;
        // Respond to client
        const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello, World!</h1>";
        send(clientSocket, response, static_cast<int>(strlen(response)), 0);
    }

    // Close socket
    closesocket(clientSocket);
}

void Server::stop() {
    closesocket(serverSocket);
}