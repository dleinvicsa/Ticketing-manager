#pragma once
#include <string>
struct Ticket {
    std::string ticketId;
    int userId;
    std::string tripId;
    std::string vehicleInfo;
    double price;
    std::string purchasedAt;
    std::string validUntil; 
    bool isUsed = false;    
};
