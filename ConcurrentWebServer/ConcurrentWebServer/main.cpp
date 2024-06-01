#include <iostream>

#include "Server.h"

int main()
{
    Server server("127.0.0.1", 8080);
    server.start();
    return 0;
}