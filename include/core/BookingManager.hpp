#ifndef BOOKINGMANAGER_HPP
#define BOOKINGMANAGER_HPP
#include <string>
#include <vector>
#include <optional>
#include "Ticket.hpp"
#include "Database.hpp"
#include "../transport/Transport.h"
#include "../transport/Trip.h"
#include "../auth/User.h"
class BookingManager {
public:
    explicit BookingManager(Database& db);
    void addTrip(const Trip& t);
    std::vector<Trip> getTrips(const std::string& filter = "ALL") const;
    bool bookRigaTicket(User& user, const std::string& tripId, RigaTicketType ticketType);
    bool bookTrainTicket(User& user, const std::string& tripId, const std::string& from, const std::string& to);
    bool useTicket(const std::string& ticketId);
    std::vector<Ticket> getUserTickets(int userId);
private:
    Database&         db_;
    std::vector<Trip> trips_;
    Trip*       findTrip(const std::string& tripId);
    std::string generateTicketId() const;
};
#endif
