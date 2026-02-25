#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include "Database.h"

class Server {
private:
    SOCKET serverSocket;
    std::vector<SOCKET> clients;   // La liste de tous les utilisateurs connectés
    std::mutex clients_mutex;      // Feu rouge pour protéger la liste quand quelqu'un se connecte/déconnecte
    Database& db;                  // Une référence vers notre base de données

    void handleClient(SOCKET clientSocket);
    void broadcastMessage(const std::string& message, SOCKET senderSocket);

public:
    Server(int port, Database& dbRef);
    ~Server();
    void start();
};

#endif