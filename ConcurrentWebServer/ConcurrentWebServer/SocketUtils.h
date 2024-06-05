#pragma once

typedef unsigned long long SOCKET;

namespace SocketUtils {
    SOCKET duplicateSocket(const SOCKET& srcSock);
}