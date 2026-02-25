#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <mutex>
#include <mysql.h>

class Database {
private:
    MYSQL* conn;           
    std::mutex db_mutex;   
public:
    Database();
    ~Database();

    bool connect(const std::string& host, const std::string& user, const std::string& pass, const std::string& dbname, int port);
    bool insertMessage(int sender_id, int receiver_id, const std::string& content);
};

#endif