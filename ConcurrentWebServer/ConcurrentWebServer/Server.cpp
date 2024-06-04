#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"

#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

Server::Server(const std::string& address, int port, int threadsCount)
    : address(address)
    , port(port)
    , threadsCount(threadsCount)
    , threadPool(threadsCount)
{
}

Server::~Server()
{
    stop();
}

SOCKET duplicateSocket(const SOCKET& srcSock) {
    WSAPROTOCOL_INFO info;
    SOCKET duplicateSock = INVALID_SOCKET;
    if (WSADuplicateSocket(srcSock, GetCurrentProcessId(), &info) == 0) {
        duplicateSock = WSASocket(info.iAddressFamily, info.iSocketType, info.iProtocol, &info, 0, 0);
    }
    return duplicateSock;
}

Server::Server(const Server& other) 
    : serverSocket(INVALID_SOCKET)
    , threadPool(other.threadsCount)
    , threadsCount(other.threadsCount)
    , address(other.address)
    , port(other.port)
{
    serverSocket = duplicateSocket(other.serverSocket);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket duplication failed with error: " << WSAGetLastError() << "\n";
    }
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
    requestHandler.handleRequest(clientSocket);
    // Close socket
    closesocket(clientSocket);
}

void Server::stop() {
    closesocket(serverSocket);
    WSACleanup();
}