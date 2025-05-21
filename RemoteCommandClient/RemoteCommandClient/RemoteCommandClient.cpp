
// Remote Command Client
// A minimal Windows console app that sends a command to the Server running in WinSIM or A20


#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(int argc, char* argv[])
{
    PCSTR SERVERIP = "127.0.0.1";
    if (argc == 1)
    {
        cout << "Server IP Address is missing. \nUsage: RemoteCommandClient <ServerIP> \n\nUsing Default Loopback IP (assuming server is running in the same machine)\n";
    }
    else if (argc == 2)
    {
        SERVERIP = argv[1];
    }
    else
    {
        cout << "Usage: RemoteCommandClient <ServerIP> \n";
        return 0;
    }

    
    cout << "Server Address: " << SERVERIP << endl;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
   
    inet_pton(AF_INET, SERVERIP, &serverAddr.sin_addr); 
    //inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); 
    //inet_pton(AF_INET, "10.160.8.54", &serverAddr.sin_addr); // Change to server IP if needed


    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        int err = WSAGetLastError();
        cerr << "Connect failed. WSA Error: " << err << "\n";
        cout << "Check if server is running!\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }


    string cmd;
    do
    {
        cout << "Enter command (e.g., time, echo something, vmdbg command, exit): ";
        
        getline(cin, cmd);
        if (cmd.empty())
            continue;

        printf("Client command: %s\n", cmd.c_str());

        send(sock, cmd.c_str(), (int)cmd.length(), 0);

        char buffer[1024] = { 0 };
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            cout << "Server response: " << buffer << "\n\n";
        }
    } while (cmd != "exit");

    closesocket(sock);
    WSACleanup();
    return 0;
}

