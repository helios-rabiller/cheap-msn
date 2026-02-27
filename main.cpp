#include "Server.h"

#include "Database.h"

#include <iostream>

using namespace std;

int main() {
    cout << "--- DEMARRAGE DU SERVEUR MSN ---" << endl;

    Database db;
    if (!db.connect("mysql-cheap-msn-cheap-msn.d.aivencloud.com", "avnadmin", "AVNS_7JbBHX7Ah1acptOnPf9", "defaultdb", 22408)){

        return 1; // Arrêt si la BDD plante
    }
    Server chatServer(8080, db);
    
    chatServer.start();

    return 0;
}