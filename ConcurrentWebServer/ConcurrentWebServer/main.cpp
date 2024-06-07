#include <iostream>

#include "Server.h"
#include "Client.h"

int main()
{
    std::string serverAddress = "127.0.0.1";
    int port = 8080;
    Server server(serverAddress, port, std::thread::hardware_concurrency() - 1);

    std::thread serverThread([&server] { server.start(); });

    std::unique_lock<std::mutex> lock(server.serverMutex);
    server.serverCV.wait(lock, [&server] {return server.IsRunning(); });

    Client client(serverAddress, port);
    if (client.connectToServer()) {
        std::cout << "Client connected to server\n";
        std::string request = "Hello from client";
        std::string response = client.sendRequest(request);
        std::cout << "Client received response: " << response << std::endl;
    }
    else {
        std::cerr << "Failed to connect to server\n";
    }

    server.stop();
    if(serverThread.joinable())
        serverThread.join();

    return 0;
}