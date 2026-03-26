#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <string>
#include <vector>
#include "../auth/User.h"
#include "Ticket.hpp"
#include "nlohmann/json.hpp"
using json = nlohmann::json;
class Database {
public:
    explicit Database(const std::string& usersPath, const std::string& ticketsPath);
    bool              updateUser(const User& u);
    User*             findUserById(int id);
    std::vector<User> allUsers();
    bool              deleteUser(int id);
    bool                saveTicket(const Ticket& t);
    bool                updateTicket(const Ticket& updated);
    bool                removeTicket(const std::string& ticketId);
    void                removeTicketsForUser(int userId);
    std::vector<Ticket> ticketsForUser(int userId);
    void                reload();
    std::vector<Ticket>& allTickets();
private:
    std::string usersFile_;
    std::string ticketsFile_;
    void loadUsers();
    void persistUsers();
    std::vector<User>   users_;
    std::vector<Ticket> tickets_;
    void loadTickets();
    void persistTickets();
};
#endif
