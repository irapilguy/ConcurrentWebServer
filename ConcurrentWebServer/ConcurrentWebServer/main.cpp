#include <iostream>

#include "Server.h"
#include "Client.h"

int main()
{
    std::string serverAddress = "127.0.0.1";
    int port = 8080;
    Server server(serverAddress, port, std::thread::hardware_concurrency() - 1);

    //server.start();
   std::thread serverThread([&server] { server.start(); });

    // Give the server a moment to start up
    std::this_thread::sleep_for(std::chrono::seconds(2));

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