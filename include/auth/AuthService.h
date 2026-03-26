#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H
#include <vector>
#include <string>
#include "User.h"
class AuthService {
private:
    std::vector<User> users;
    std::string usersFilePath;
    std::string generateSalt();
    int generateNextId();
    bool saveUsers();
public:
    bool loadUsers(const std::string& filePath);
    std::string hashPassword(const std::string& password, const std::string& salt);
    bool verifyPassword(const std::string& inputPassword, const User& user);
    User* findUserByLogin(const std::string& login);
    bool login(const std::string& login, const std::string& password);
    bool registerUser(const std::string& login, const std::string& password, const std::string& role = "user");
    bool changePassword(int userId, const std::string& newPassword);
    bool deleteUser(int userId);
};
#endif