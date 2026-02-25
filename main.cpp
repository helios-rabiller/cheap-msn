#include "Server.h"

#include "Database.h"

#include <iostream>

using namespace std;

int main() {
    cout << "--- DEMARRAGE DU SERVEUR MSN ---" << endl;

    Database db;
    if (!db.connect("127.0.0.1", "adminmsn", "aaa", "msn", 3306)) {
        return 1; // Arrêt si la BDD plante
    }
    Server chatServer(8080, db);
    
    chatServer.start();

    return 0;
}