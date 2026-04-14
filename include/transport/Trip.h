#ifndef TRIP_H
#define TRIP_H
#include <string>
#include <iostream>
#include "Transport.h"
struct Trip {
    std::string tripId;           
    Transport* vehicle;            
    std::string fromCity;          
    std::string toCity;            
    std::string departureTime;     
    std::string tripType;          
    Trip(std::string id, Transport* v, std::string from, std::string to, std::string time) {
        tripId = id;
        vehicle = v;
        fromCity = from;
        toCity = to;
        departureTime = time;
        tripType = "RIGA";
    }
    Trip(std::string id, Transport* v, std::string from, std::string to, std::string time, std::string type) {
        tripId = id;
        vehicle = v;
        fromCity = from;
        toCity = to;
        departureTime = time;
        tripType = type;  
    }
    double getPrice(bool hasStudentCard, RigaTicketType ticketType) {
        if (tripType == "RIGA") {
            return vehicle->calculatePrice(hasStudentCard, ticketType);
        } else {
            return vehicle->calculateTrainPrice(hasStudentCard, fromCity, toCity);
        }
    }
    void printInfo() {
        std::cout << tripId << ": " << vehicle->getInfo() << std::endl;
        std::cout << "   " << fromCity << " -> " << toCity;
        std::cout << ", time: " << departureTime << std::endl;
    }
};
#endif
