#include "Client.h"
#include "SocketUtils.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

Client::Client(const std::string& serverAddress, int port)
    : serverAddress(serverAddress)
    , port(port)
    , connectSocket(INVALID_SOCKET)
{}

Client::~Client() {
    if (connectSocket != INVALID_SOCKET) {
        closesocket(connectSocket);
        WSACleanup();
    }
}

Client::Client(const Client& other) 
    : serverAddress(other.serverAddress)
    , port(other.port)
    , connectSocket(INVALID_SOCKET)
{
    connectSocket = SocketUtils::duplicateSocket(other.connectSocket);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "Socket duplication failed with error: " << WSAGetLastError() << "\n";
    }
}

bool Client::connectToServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return false;
    }

    struct addrinfo* result = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    std::string portStr = std::to_string(port);
    int iResult = getaddrinfo(serverAddress.c_str(), portStr.c_str(), &hints, &result);
    if ( iResult!= 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return false;
    }

    for (auto addrPtr = result; addrPtr != NULL; addrPtr = addrPtr->ai_next) {
        connectSocket = socket(addrPtr->ai_family, addrPtr->ai_socktype, addrPtr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
            freeaddrinfo(result);
            WSACleanup();
            return false;
        }

        if (connect(connectSocket, addrPtr->ai_addr, static_cast<int>(addrPtr->ai_addrlen)) == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }

        break;
    }

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return false;
    }

    return true;
}

std::string Client::sendRequest(const std::string& request) {

    if (send(connectSocket, request.c_str(), request.length(), 0) == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        return "send failed";
    }

    const int bufferSize = 1024;
    char buffer[bufferSize];

    int iResult = recv(connectSocket, buffer, bufferSize, 0);
    if (iResult > 0) {
        return std::string(buffer, iResult);
    }
    else if (iResult == 0) {
        return "Connection closed";
    }
    else {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
        return "recv failed";
    }
}
