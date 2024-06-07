#include "pch.h"
#include "Client.h"
#include "Server.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

#include<gtest/gtest.h>

TEST(ServerTest, BasicRequest) {

    std::string serverAddress = "127.0.0.1";
    int port = 8081;
    Server server(serverAddress, port, std::thread::hardware_concurrency());
    std::thread testServerThread([&server] { server.start(); });

    std::unique_lock<std::mutex> lock(server.serverMutex);
    server.serverCV.wait(lock, [&server] {return server.IsRunning(); });

    Client client(serverAddress, port);

    ASSERT_TRUE(client.connectToServer());

    std::string request = "Test client conncetion";
    std::string response = client.sendRequest(request);

    // Check if the response is valid
    ASSERT_TRUE(response.find("HTTP/1.1 200 OK") != std::string::npos);
    ASSERT_TRUE(response.find("<html><body><h1>Hello, World!</h1></body></html>") != std::string::npos);

    server.stop();
    if (testServerThread.joinable())
        testServerThread.join();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}