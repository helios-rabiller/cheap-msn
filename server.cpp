#include <iostream>
#include <mysql/jdbc.h>  
#include <ctime>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[1024];
    
    try {
        // Connexion à la base de données
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://127.0.0.1:3306", "adminmsn", "aaa");
        con->setSchema("msn");
        
        cout << "Connecté à la base MSN" << endl;
        
        // initialize socket (Winsock)
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cerr << "WSAStartup failed" << endl;
            return 1;
        }
        cout << "Winsock initialisé" << endl;
        
        // create a socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            cerr << "Erreur création socket: " << WSAGetLastError() << endl;
            WSACleanup();
            return 1;
        }
        cout << "Socket créée" << endl;
        
        // bind the socket to an ip addres and port
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(8080);
        
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cerr << "Erreur bind: " << WSAGetLastError() << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }
        cout << "Socket bindée sur port 8080" << endl;
        
        // tell winsock the socket is listening
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            cerr << "Erreur listen: " << WSAGetLastError() << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }
        cout << "Socket en écoute..." << endl;
        
        // wait for connection
        cout << "En attente de connexion client..." << endl;
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Erreur accept: " << WSAGetLastError() << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }
        cout << "Client connecté!" << endl;
        
        // close listening socket
        closesocket(serverSocket);
        cout << "Socket d'écoute fermée" << endl;
        
        // while loop: accept and echo message back to client
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            
            if (bytesReceived <= 0) {
                cout << "Client déconnecté ou erreur" << endl;
                break;
            }
            
            cout << "Message reçu: " << buffer << endl;
            
            // Insérer le message dans la base de données
            sql::PreparedStatement *pstmt = con->prepareStatement(
                "INSERT INTO messages (sender_id, receiver_id, content) VALUES (1, 1, ?)"
            );
            pstmt->setString(1, string(buffer));
            pstmt->executeUpdate();
            delete pstmt;
            
            // Echo back to client
            send(clientSocket, buffer, bytesReceived, 0);
            cout << "Message renvoyé au client" << endl;
        }
        
        // Close the sock
        closesocket(clientSocket);
        cout << "Socket client fermée" << endl;
        
        // Afficher les messages de la base
        cout << "\nLISTE COMPLÈTE DES MESSAGES:" << endl;
        cout << "===============================" << endl;
        
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery(
            "SELECT m.id, m.content, m.timestamp, "
            "       u_sender.username as sender, "
            "       u_receiver.username as receiver "
            "FROM messages m "
            "LEFT JOIN users u_sender ON m.sender_id = u_sender.id "
            "LEFT JOIN users u_receiver ON m.receiver_id = u_receiver.id "
            "ORDER BY m.timestamp DESC LIMIT 10"
        );
        
        while (res->next()) {
            cout << "ID: " << res->getInt("id") << endl;
            cout << "  De: " << (res->isNull("sender") ? "?" : res->getString("sender")) << endl;
            cout << "  À: " << (res->isNull("receiver") ? "?" : res->getString("receiver")) << endl;
            cout << "  Message: " << res->getString("content") << endl;
            cout << "  Date: " << res->getString("timestamp") << endl;
            cout << "  ---" << endl;
        }
        
        delete res;
        delete stmt;
        delete con;
        
        cout << "===============================" << endl;
        cout << "Terminé!" << endl;
        
        // shutdown winsock
        WSACleanup();
        cout << "Winsock arrêté" << endl;
        
    } catch (sql::SQLException &e) {
        cerr << "Erreur MySQL: " << e.what() << endl;
    }
    
    return 0;
}