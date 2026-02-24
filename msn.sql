CREATE DATABASE IF NOT EXISTS msn;
USE msn;

-- 1. Table des utilisateurs
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL, -- Stockez le hash, pas le mot de passe en clair !
    status ENUM('offline', 'online', 'busy', 'away') DEFAULT 'offline',
    ip_address VARCHAR(45), -- Pour savoir où envoyer les messages (IPv4 ou IPv6)
    socket_id INT DEFAULT 0, -- Utile pour retrouver le client connecté en mémoire vive
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 2. Table des amis (Relation Many-to-Many)
CREATE TABLE IF NOT EXISTS friends (
    user_id INT,
    friend_id INT,
    status ENUM('pending', 'accepted', 'blocked') DEFAULT 'pending',
    PRIMARY KEY (user_id, friend_id),
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (friend_id) REFERENCES users(id) ON DELETE CASCADE
);

-- 3. Table des messages (Pour l'historique et messages hors ligne)
CREATE TABLE IF NOT EXISTS messages (
    id INT AUTO_INCREMENT PRIMARY KEY,
    sender_id INT NOT NULL,
    receiver_id INT NOT NULL,
    content TEXT,
    is_wizz BOOLEAN DEFAULT FALSE, 
    sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_read BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (sender_id) REFERENCES users(id),
    FOREIGN KEY (receiver_id) REFERENCES users(id)
);