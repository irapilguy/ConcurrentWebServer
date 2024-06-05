#include "SocketUtils.h"

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

SOCKET SocketUtils::duplicateSocket(const SOCKET& srcSock)
{
    WSAPROTOCOL_INFO info;
    SOCKET duplicateSock = INVALID_SOCKET;
    if (WSADuplicateSocket(srcSock, GetCurrentProcessId(), &info) == 0) {
        duplicateSock = WSASocket(info.iAddressFamily, info.iSocketType, info.iProtocol, &info, 0, 0);
    }
    return duplicateSock;
}
