#include <iostream>
#include <stdexcept>

#include <Winsock2.h>
#include <ws2tcpip.h>

int main() {

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Your server code here

    // Cleanup Winsock
    WSACleanup();
    return 0;
}