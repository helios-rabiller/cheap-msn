#ifndef SOCK_CLIENT_WIN_HPP
#define SOCK_CLIENT_WIN_HPP

#define PORT 8080

#include <winsock.h>

class sock_client_win {

    private:
        // W I N S O C K
        int winsock_init();

    public:
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