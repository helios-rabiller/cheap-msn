#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// Fonction pour écouter les messages venant du serveur
void receive_messages(SOCKET sock) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            cout << "\n[Deconnecte du serveur]" << endl;
            exit(0);
        }
        cout << "\n[Serveur dit] : " << buffer << "\n> " << flush;
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP du serveur
    serverAddr.sin_port = htons(8080);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Impossible de se connecter au serveur." << endl;
        return 1;
    }
    cout << "Connecte au serveur MSN ! Tape tes messages." << endl;

    thread receiver(receive_messages, clientSocket);
    receiver.detach();

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);
        if (input == "quit") break;
        
        send(clientSocket, input.c_str(), input.length(), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}