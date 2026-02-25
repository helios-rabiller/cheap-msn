#include "Server.h"
#include <iostream>
#include <ws2tcpip.h>
#include <mysql.h> 

using namespace std;

Server::Server(int port, Database& dbRef) : db(dbRef) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    cout << "[Serveur] Initialise et en ecoute sur le port " << port << endl;
}

Server::~Server() {
    closesocket(serverSocket);
    WSACleanup();
}

void Server::broadcastMessage(const string& message, SOCKET senderSocket) {
    lock_guard<mutex> lock(clients_mutex); 
    for (SOCKET client : clients) {
        send(client, message.c_str(), message.length(), 0);
    }
}

void Server::handleClient(SOCKET clientSocket) {
    mysql_thread_init(); 

    char buffer[1024];
    cout << "[Serveur] Client connecte. ID Socket : " << clientSocket << endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            cout << "[Serveur] Client deconnecte. ID : " << clientSocket << endl;
            
            lock_guard<mutex> lock(clients_mutex);
            for (auto it = clients.begin(); it != clients.end(); ++it) {
                if (*it == clientSocket) {
                    clients.erase(it);
                    break;
                }
            }
            break;
        }

        string msg(buffer);
        cout << "[Recu de " << clientSocket << "] : " << msg << endl;

        db.insertMessage(1, 1, msg);

        string broadcastMsg = "Client " + to_string(clientSocket) + " : " + msg;
        broadcastMessage(broadcastMsg, clientSocket);
    }

    mysql_thread_end(); // clear thread
    closesocket(clientSocket);
}


void Server::start() {
    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket != INVALID_SOCKET) {
            {
                lock_guard<mutex> lock(clients_mutex);
                clients.push_back(clientSocket);
            }

            thread t(&Server::handleClient, this, clientSocket);
            t.detach();
        }
    }
}