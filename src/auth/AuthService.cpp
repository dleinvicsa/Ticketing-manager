#include "AuthService.h"
#include <fstream>
#include <iostream>
#include <functional>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <random>
#include <json.hpp>
using json = nlohmann::json;
bool AuthService::loadUsers(const std::string& filePath) {
    usersFilePath = filePath;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Failed to open users.json\n";
        return false;
    }
    json data;
    file >> data;
    for (auto& u : data["users"]) {
        User user(
            u["id"],
            u["login"],
            u["password_hash"],
            u["salt"],
            u["balance"],
            u["role"],
            u["created_at"]
        );
        users.push_back(user);
    }
    return true;
}
std::string AuthService::hashPassword(const std::string& password, const std::string& salt) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(password + salt));
}
bool AuthService::verifyPassword(const std::string& inputPassword, const User& user) {
    std::string hashed = hashPassword(inputPassword, user.salt);
    return hashed == user.password_hash;
}
User* AuthService::findUserByLogin(const std::string& login) {
    for (auto& user : users) {
        if (user.login == login) {
            return &user;
        }
    }
    return nullptr;
}
bool AuthService::login(const std::string& login, const std::string& password) {
    User* user = findUserByLogin(login);
    if (!user) {
        std::cout << "User not found\n";
        return false;
    }
    if (verifyPassword(password, *user)) {
        std::cout << "Login successful\n";
        return true;
    }
    std::cout << "Invalid password\n";
    return false;
}
std::string AuthService::generateSalt() {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, sizeof(charset) - 2);
    std::string salt;
    salt.reserve(16);
    for (int i = 0; i < 16; ++i) {
        salt += charset[dist(rng)];
    }
    return salt;
}
int AuthService::generateNextId() {
    int maxId = 0;
    for (const auto& user : users) {
        if (user.id > maxId) maxId = user.id;
    }
    return maxId + 1;
}
bool AuthService::saveUsers() {
    if (usersFilePath.empty()) {
        std::cout << "No file path set, cannot save users\n";
        return false;
    }
    json data;
    data["users"] = json::array();
    for (const auto& user : users) {
        data["users"].push_back({
            {"id",            user.id},
            {"login",         user.login},
            {"password_hash", user.password_hash},
            {"salt",          user.salt},
            {"balance",       user.balance},
            {"role",          user.role},
            {"created_at",    user.created_at}
        });
    }
    std::ofstream file(usersFilePath);
    if (!file.is_open()) {
        std::cout << "Failed to open file for writing\n";
        return false;
    }
    file << data.dump(4);
    return true;
}
bool AuthService::registerUser(const std::string& login, const std::string& password, const std::string& role) {
    if (login.empty() || password.empty()) {
        std::cout << "Login and password must not be empty\n";
        return false;
    }
    if (findUserByLogin(login)) {
        std::cout << "User with this login already exists\n";
        return false;
    }
    std::string salt = generateSalt();
    std::string passwordHash = hashPassword(password, salt);
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm tmBuf;
#ifdef _WIN32
    gmtime_s(&tmBuf, &nowTime);
#else
    gmtime_r(&nowTime, &tmBuf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tmBuf, "%Y-%m-%dT%H:%M:%SZ");
    User newUser(
        generateNextId(),
        login,
        passwordHash,
        salt,
        0.0,
        role,
        oss.str()
    );
    users.push_back(newUser);
    if (!saveUsers()) {
        users.pop_back(); 
        std::cout << "Failed to persist new user\n";
        return false;
    }
    std::cout << "User '" << login << "' registered successfully\n";
    return true;
}
bool AuthService::changePassword(int userId, const std::string& newPassword) {
    for (auto& user : users) {
        if (user.id == userId) {
            user.salt = generateSalt();
            user.password_hash = hashPassword(newPassword, user.salt);
            return saveUsers();
        }
    }
    return false;
}
bool AuthService::deleteUser(int userId) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->id == userId) {
            if (it->login == "admin") {
                std::cout << "Cannot delete admin account\n";
                return false;
            }
            users.erase(it);
            return saveUsers();
        }
    }
    return false;
}