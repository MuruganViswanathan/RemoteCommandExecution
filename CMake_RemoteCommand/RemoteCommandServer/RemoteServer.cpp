// RemoteServer.cpp : Defines the entry point for the application.
//
#include "RemoteServer.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstring>


#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define CLOSESOCKET closesocket
#define SOCKET_TYPE SOCKET
#define SOCKET_ERROR_CODE SOCKET_ERROR

#else 

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#define CLOSESOCKET close
#define SOCKET_TYPE int
#define SOCKET_ERROR_CODE -1

#endif

using namespace std;

static void StartRemoteServer(UShell* shell, int port = 9000);
static std::string ProcessSimpleCommand(const std::string& input);



int main()
{
	cout << "Hello RemoteServer" << endl;

    UShell ushell;
    ushell.Run();

    std::cout << "Press Enter to exit...\n";
    std::cin.get();
    return 0;
}


// -- UShell Implementation --

void UShell::Run()
{
    FileLogger file;
    file.log("[REMOTECOMMAND] UShell::Run()\n");

    file.log("[REMOTECOMMAND] UShell::Run() calling StartRemoteControl()");
    StartRemoteServer(this, 9000);

}


// [REMOTECOMMAND] Remote Command execution for Data Acquisition   
std::string UShell::ExecuteCommandFromRemote(const std::string& input)
{
    FileLogger file;
    file.log("[REMOTECOMMAND] UShell::ExecuteCommandFromRemote\n");

    //------------- Test Start --------------
    // Remote Commands - special test commands like echo, time, for testing remote command.
    // forward it to real command parser and execute for non-test commands.

    std::string result = ProcessSimpleCommand(input);
    if (result != "AdvancedCommand")
    {
        file.log(result.c_str());
        return result;
    }
    else
    {
        file.log("[REMOTECOMMAND] UShell::ExecuteCommandFromRemote() Processing advancd command: '" + input + "' not supported in Test");
    }

    return result;
    //------------- Test End. ----------------
}


// ===========================================

// sample simple commands for testing the remote command 
static std::string ProcessSimpleCommand(const std::string& input)
{
    FileLogger file;
    char buffer[128];
    std::memset(buffer, 0, 128);
    std::snprintf(buffer, 128, "%s %s", "[REMOTECOMMAND] ProcessSimpleCommand ", input.c_str());
    file.log(buffer);

    // Extract sample test command and optional argument
    std::istringstream iss(input);
    std::string cmd, arg;
    if (!(iss >> cmd))
    {
        // No command found, send response string to remote client
        return "Error: No command received\n";
    }

    // Get the rest of the line as arguments
    std::getline(iss, arg);
    if (!arg.empty() && arg[0] == ' ')
        arg = arg.substr(1);  // remove leading space

    // Execute if sample command found
    std::ostringstream oss;
    if (cmd == "echo")
    {
        file.log("[REMOTECOMMAND] ProcessSimpleCommand echo");
        file.log(arg);
        oss << arg;  //echo back the string to remote client
    }
    else if (cmd == "time")
    {
        file.log("[REMOTECOMMAND] ProcessSimpleCommand time");
        time_t now = time(nullptr);
        oss << "Current time: " << ctime(&now);
    }
    else if (cmd == "exit")
    {
        file.log("[REMOTECOMMAND] ProcessSimpleCommand exit");
        oss << "Exiting shell...";
    }
    else
    {
        file.log("[REMOTECOMMAND] Process advanced command...");
        return "AdvancedCommand";
    }

    std::memset(buffer, 0, 128);
    std::snprintf(buffer, 128, "%s %s", "[REMOTECOMMAND] ProcessSimpleCommand returned", oss.str().c_str());
    file.log(buffer);

    return oss.str();
}

// ===========================================================
// helpers to initialize sockets
void InitSockets()
{
    FileLogger file;
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        file.log("[REMOTECOMMAND] WSAStartup failed!!");
    }
#endif
}

void CleanupSockets()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

// ===========================================================
// REMOTECOMMAND: -- Simple TCP Server to Listen for Remote Commands

static void StartRemoteServer(UShell* shell, int port)
{
    FileLogger file;
    file.log("[REMOTECOMMAND] StartRemoteServer");

    InitSockets();

    std::thread([shell, port]()
        {
            FileLogger file;

            SOCKET_TYPE serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (serverSocket == SOCKET_ERROR_CODE)
            {

                file.log("[REMOTECOMMAND] StartRemoteServer: Socket creation failed!!");

                CleanupSockets();
                return;
            }

            struct sockaddr_in address;
            int addrlen = sizeof(address);
            memset(&address, 0, sizeof(address));
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
            address.sin_port = htons(port);

            if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR_CODE)
            {
                file.log("[REMOTECOMMAND] StartRemoteServer: Bind failed!!");

                CLOSESOCKET(serverSocket);
                CleanupSockets();
                return;
            }

            if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR_CODE)
            {

                file.log("[REMOTECOMMAND] StartRemoteServer: Listen failed!!");

                CLOSESOCKET(serverSocket);
                CleanupSockets();
                return;
            }

            std:ostringstream oss;
            oss << "[REMOTECOMMAND] StartRemoteServer: Server is running on port " << port;
            string tempbuf = oss.str();
            file.log(tempbuf.c_str());


            // Run forever until Exit command
            while (true)
            {
                file.log("[REMOTECOMMAND] Waiting for the remote command....\n");

#ifdef _WIN32
                SOCKET_TYPE clientSocket = accept(serverSocket, (SOCKADDR*)&address, &addrlen);
#else
                SOCKET_TYPE clientSocket = accept(serverSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
#endif
                if (clientSocket == SOCKET_ERROR_CODE)
                {
                    file.log("[REMOTECOMMAND] Accept failed, continuing...\n");
                    continue;
                }

                // process multiple command from the same client in a loop (until the 'exit' command)
                while (true)
                {
                    char buffer[1024] = { 0 };
#ifdef _WIN32
                    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
#else
                    int bytesReceived = read(clientSocket, buffer, sizeof(buffer));
#endif
                    if (bytesReceived <= 0)
                    {
                        file.log("[REMOTECOMMAND] Connection closed or error.\n");
                        break;
                    }

                    std::string command(buffer);
                    file.log("[REMOTECOMMAND] StartRemoteServer; Received command: " + command);

                    std::string result = shell->ExecuteCommandFromRemote(command);

                    send(clientSocket, result.c_str(), (int)result.size(), 0);


                    std::string temp = "[REMOTECOMMAND] StartRemoteServer: sent back to client: " + result;
                    file.log(temp.c_str());

                    // check for the 'exit' command to break out of the per client loop
                    if (command == "exit")
                    {
                        file.log("[REMOTECOMMAND] StartRemoteServer: Exit command received. Close Client Socket\n");
                        break;
                    }
                }

                CLOSESOCKET(clientSocket);

            }

            CLOSESOCKET(serverSocket);
            CleanupSockets();

        }).detach();  // Run in background
}



