// Remote Command server - Receives commands from a Remote Client (could be running on a seperate machine)
// using Socket connection PORt 9000. Executes the command and sends the result back to the client
// Author: Murugan Viswanathan

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>


#pragma comment(lib, "ws2_32.lib")

void HandleClient(SOCKET clientSocket) 
{
    std::cout << "HandleClient started" << std::endl;

    char buffer[1024] = {0};

    while (true)
    {
        // clear buffer everytime a new command is received
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0)
        {
            std::cerr << "bytesReceived <= 0. Connection closed or error.\n";
            closesocket(clientSocket);
            break;
        }

        std::string command(buffer);
        std::string response;

        std::cout << "Command Received: " << command << std::endl;

        // Simulate basic shell commands
        if (command.substr(0, 5) == "echo ")
        {
            response = command.substr(5); // Everything after 'echo '
        }
        else if (command == "time")
        {
            time_t now = time(nullptr);
            char timebuf[64];
            ctime_s(timebuf, sizeof(timebuf), &now);
            response = std::string("Current time: ") + timebuf;
        }
        else if (command == "exit")
        {
            response = "Server received exit command. Closing connection.";
            send(clientSocket, response.c_str(), (int)response.length(), 0);
            break; // Exit the loop, close the socket
        }
        else
        {
            response = "Unknown command: " + command;
        }

        send(clientSocket, response.c_str(), (int)response.length(), 0);
    }
     
    closesocket(clientSocket);
    std::cout << "Connection Closed" << std::endl;
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    serverAddr.sin_port = htons(9000);

    if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is running on port 9000...\n";

    while (true) 
    {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed.\n";
            continue;
        }

        std::thread(HandleClient, clientSocket).detach(); // Handle in separate thread
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}

