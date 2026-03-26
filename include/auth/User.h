#ifndef USER_H
#define USER_H
#include <string>
class User {
public:
    int id;
    std::string login;
    std::string password_hash;
    std::string salt;
    double balance;
    std::string role;
    std::string created_at;
    User() = default;
    User(int id,
        const std::string& login,
        const std::string& password_hash,
        const std::string& salt,
        double balance,
        const std::string& role,
        const std::string& created_at);
};
#endif