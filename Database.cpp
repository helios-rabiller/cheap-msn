#include "Database.h"
#include <iostream>

using namespace std;

Database::Database() {
    conn = mysql_init(NULL);
}

Database::~Database() {
    if (conn) {
        mysql_close(conn);
        cout << "[Database] Connexion MySQL fermee proprement." << endl;
    }
}

bool Database::connect(const string& host, const string& user, const string& pass, const string& dbname, int port) {
    
    unsigned int ssl_mode = SSL_MODE_REQUIRED;
    mysql_options(conn, MYSQL_OPT_SSL_MODE, &ssl_mode);

    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), pass.c_str(), dbname.c_str(), port, NULL, 0)) {
        cerr << "[Database] Erreur de connexion : " << mysql_error(conn) << endl;
        return false;
    }
    cout << "[Database] Connecte a la BDD Cloud avec succes !" << endl;
    return true;
}

bool Database::insertMessage(int sender_id, int receiver_id, const string& content) {

    lock_guard<mutex> lock(db_mutex);

    char safe_msg[2048];
    mysql_real_escape_string(conn, safe_msg, content.c_str(), content.length());

    string query = "INSERT INTO messages (sender_id, receiver_id, content) VALUES (" 
                   + to_string(sender_id) + ", " 
                   + to_string(receiver_id) + ", '" 
                   + string(safe_msg) + "')";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[Database] Erreur SQL : " << mysql_error(conn) << endl;
        return false;
    }
    return true;
}