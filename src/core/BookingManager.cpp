#include "BookingManager.hpp"
#include <ctime>
#include <iostream>
BookingManager::BookingManager(Database& db) : db_(db) {}
void BookingManager::addTrip(const Trip& t) {
    trips_.push_back(t);
}
std::vector<Trip> BookingManager::getTrips(const std::string& filter) const {
    if (filter == "ALL") return trips_;
    std::vector<Trip> result;
    for (const auto& t : trips_) {
        if (t.tripType == filter) result.push_back(t);
    }
    return result;
}
Trip* BookingManager::findTrip(const std::string& tripId) {
    for (auto& t : trips_) {
        if (t.tripId == tripId) return &t;
    }
    return nullptr;
}
std::string BookingManager::generateTicketId() const {
    return "TKT-" + std::to_string(std::time(0)) + "-" + std::to_string(std::rand() % 1000);
}
bool BookingManager::bookRigaTicket(User& user, const std::string& tripId, RigaTicketType ticketType) {
    Trip* trip = findTrip(tripId);
    if (!trip) return false;
    double price = trip->vehicle->calculatePrice(user.role == "student", ticketType);
    if (user.balance < price) {
        std::cout << "Insufficient balance!\n";
        return false;
    }
    user.balance -= price;
    db_.updateUser(user);
    Ticket t;
    t.ticketId = generateTicketId();
    t.userId = user.id;
    t.tripId = tripId;
    std::string typeInfo = "";
    if (ticketType == RigaTicketType::SINGLE_90MIN) typeInfo = "90 min";
    else if (ticketType == RigaTicketType::DAILY_24H) typeInfo = "24h";
    else if (ticketType == RigaTicketType::DAILY_72H) typeInfo = "72h";
    else if (ticketType == RigaTicketType::DAILY_120H) typeInfo = "120h";
    else if (ticketType == RigaTicketType::MONTHLY_30D) typeInfo = "30d";
    t.vehicleInfo = trip->vehicle->getInfo() + " [" + typeInfo + "]";
    t.price = price;
    t.purchasedAt = "2024-03-20 12:00"; 
    return db_.saveTicket(t);
}
bool BookingManager::bookTrainTicket(User& user, const std::string& tripId, const std::string& from, const std::string& to) {
    Trip* trip = findTrip(tripId);
    if (!trip) return false;
    double price = trip->vehicle->calculateTrainPrice(user.role == "student", from, to);
    if (user.balance < price) {
        std::cout << "Insufficient balance!\n";
        return false;
    }
    user.balance -= price;
    db_.updateUser(user);
    Ticket t;
    t.ticketId = generateTicketId();
    t.userId = user.id;
    t.tripId = tripId;
    t.vehicleInfo = trip->vehicle->getInfo() + " (" + from + " -> " + to + ")";
    t.price = price;
    t.purchasedAt = "2024-03-18 12:00";
    return db_.saveTicket(t);
}
bool BookingManager::useTicket(const std::string& ticketId)
{
    for (auto& t : db_.allTickets()) {
        if (t.ticketId == ticketId && !t.isUsed) {
            t.isUsed = true;
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            std::tm *ptm = std::localtime(&now_c);
            if (t.vehicleInfo.find("Train") != std::string::npos || t.vehicleInfo.find("VIVI") != std::string::npos) {
                t.validUntil = "-";
            } else {
                if (t.vehicleInfo.find("90") != std::string::npos) {
                    ptm->tm_min += 90;
                } else if (t.vehicleInfo.find("24h") != std::string::npos || t.vehicleInfo.find("24") != std::string::npos) {
                    ptm->tm_hour += 24;
                } else if (t.vehicleInfo.find("72h") != std::string::npos || t.vehicleInfo.find("3 day") != std::string::npos || t.vehicleInfo.find("72") != std::string::npos) {
                    ptm->tm_hour += 72;
                } else if (t.vehicleInfo.find("120h") != std::string::npos || t.vehicleInfo.find("5 day") != std::string::npos || t.vehicleInfo.find("120") != std::string::npos) {
                    ptm->tm_hour += 120;
                } else if (t.vehicleInfo.find("30d") != std::string::npos || t.vehicleInfo.find("Monthly") != std::string::npos || t.vehicleInfo.find("30") != std::string::npos) {
                    ptm->tm_mday += 30;
                } else {
                    ptm->tm_min += 90;
                }
                std::mktime(ptm);
                char buffer[64];
                std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", ptm);
                t.validUntil = buffer;
            }
           db_.updateTicket(t);
            return true;
        }
    }
    return false;
}
std::vector<Ticket> BookingManager::getUserTickets(int userId) {
    return db_.ticketsForUser(userId);
}
