#include "Database.hpp"
#include <fstream>
#include <iostream>
Database::Database(const std::string& usersPath, const std::string& ticketsPath) 
    : usersFile_(usersPath), ticketsFile_(ticketsPath) {
    loadUsers();
    loadTickets();
}
void Database::loadUsers() {
    std::ifstream file(usersFile_);
    if (!file || file.peek() == std::ifstream::traits_type::eof()) return;
    json data;
    try {
        file >> data;
        users_.clear();
        for (auto& u : data["users"]) {
            users_.push_back(User(u["id"], u["login"], u["password_hash"], u["salt"], 
                            u["balance"], u["role"], u["created_at"]));
        }
    } catch (...) {}
}
void Database::persistUsers() {
    json data;
    data["users"] = json::array();
    for (const auto& u : users_) {
        json j;
        j["id"] = u.id;
        j["login"] = u.login;
        j["password_hash"] = u.password_hash;
        j["salt"] = u.salt;
        j["balance"] = u.balance;
        j["role"] = u.role;
        j["created_at"] = u.created_at;
        data["users"].push_back(j);
    }
    std::ofstream file(usersFile_);
    file << data.dump(4);
}
void Database::loadTickets() {
    std::ifstream file(ticketsFile_);
    if (!file || file.peek() == std::ifstream::traits_type::eof()) return;
    json data;
    try {
        file >> data;
        tickets_.clear();
        for (auto& t : data["tickets"]) {
            Ticket ticket;
            ticket.ticketId = t["ticketId"];
            ticket.userId = t["userId"];
            ticket.tripId = t["tripId"];
            ticket.vehicleInfo = t["vehicleInfo"];
            ticket.price = t["price"];
            ticket.purchasedAt = t["purchasedAt"];
            ticket.validUntil = t.value("validUntil", "");
            ticket.isUsed = t.value("isUsed", false);
            tickets_.push_back(ticket);
        }
    } catch (...) {}
}
bool Database::updateTicket(const Ticket& updated)
{
    for (auto& t : tickets_) {
        if (t.ticketId == updated.ticketId) {
            t = updated;
            persistTickets();
            return true;
        }
    }
    return false;
}
void Database::persistTickets() {
    json data;
    data["tickets"] = json::array();
    for (const auto& t : tickets_) {
        json j;
        j["ticketId"] = t.ticketId;
        j["userId"] = t.userId;
        j["tripId"] = t.tripId;
        j["vehicleInfo"] = t.vehicleInfo;
        j["price"] = t.price;
        j["purchasedAt"] = t.purchasedAt;
        j["validUntil"] = t.validUntil;
        j["isUsed"] = t.isUsed;
        data["tickets"].push_back(j);
    }
    std::ofstream file(ticketsFile_);
    file << data.dump(4);
}
bool Database::updateUser(const User& u) {
    for (auto& user : users_) {
        if (user.id == u.id) {
            user.balance = u.balance;
            persistUsers();
            return true;
        }
    }
    return false;
}
User* Database::findUserById(int id) {
    for (auto& user : users_) {
        if (user.id == id) return &user;
    }
    return nullptr;
}
std::vector<User> Database::allUsers() {
    return users_;
}
bool Database::deleteUser(int id) {
    for (auto it = users_.begin(); it != users_.end(); ++it) {
        if (it->id == id) {
            users_.erase(it);
            persistUsers();
            return true;
        }
    }
    return false;
}
bool Database::saveTicket(const Ticket& t) {
    tickets_.push_back(t);
    persistTickets();
    return true;
}
bool Database::removeTicket(const std::string& ticketId) {
    for (auto it = tickets_.begin(); it != tickets_.end(); ++it) {
        if (it->ticketId == ticketId) {
            tickets_.erase(it);
            persistTickets();
            return true;
        }
    }
    return false;
}
void Database::removeTicketsForUser(int userId) {
    bool changed = false;
    for (auto it = tickets_.begin(); it != tickets_.end(); ) {
        if (it->userId == userId) {
            it = tickets_.erase(it);
            changed = true;
        } else {
            ++it;
        }
    }
    if (changed) persistTickets();
}
std::vector<Ticket>& Database::allTickets() {
    return tickets_;
}
std::vector<Ticket> Database::ticketsForUser(int userId) {
    std::vector<Ticket> result;
    for (const auto& t : tickets_) {
        if (t.userId == userId) result.push_back(t);
    }
    return result;
}
void Database::reload() {
    loadUsers();
    loadTickets();
}
