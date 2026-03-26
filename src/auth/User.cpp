#include "User.h"
User::User(int id,
    const std::string& login,
    const std::string& password_hash,
    const std::string& salt,
    double balance,
    const std::string& role,
    const std::string& created_at)
    : id(id),
    login(login),
    password_hash(password_hash),
    salt(salt),
    balance(balance),
    role(role),
    created_at(created_at) {
}