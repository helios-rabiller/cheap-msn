#ifndef SOCK_CLIENT_WIN_HPP
#define SOCK_CLIENT_WIN_HPP

#define PORT 8080
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

class sock_client_win {

    private:
        WSADATA wsaData;
        int iResult;
        SOCKET ConnectSocket = INVALID_SOCKET;
        struct addrinfo 
                *result = NULL,
                *ptr = NULL,
                hints;

        int recvbuflen = DEFAULT_BUFLEN;

        const char *sendbuf = "this is a test";
        char recvbuf[DEFAULT_BUFLEN];
        // W I N S O C K
        int winsock_init();

    public:
        sock_client_win() {
            iResult = winsock_init();
        }

        // S O C K E T
        int socket_init();

        // C O N N E C T
        int socket_connect();
    
        // S E N D  /  R E C E I V E
        int socket_send();
        int socket_receive();

        // D I S C O N N E C T
        int socket_disconnect();
};

#endif