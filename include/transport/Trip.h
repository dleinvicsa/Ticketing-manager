#ifndef TRIP_H
#define TRIP_H

#include <string>
#include <iostream>
#include "Transport.h"

// A trip = from, to, time, which vehicle?
struct Trip {
    std::string tripId;           // Trip ID
    Transport* vehicle;            // Which vehicle?
    std::string fromCity;          // Departure city
    std::string toCity;            // Arrival city
    std::string departureTime;     // Departure time
    std::string tripType;          // "RIGA" or "VIVI"
    
    // Constructor 
    Trip(std::string id, Transport* v, std::string from, std::string to, std::string time) {
        tripId = id;
        vehicle = v;
        fromCity = from;
        toCity = to;
        departureTime = time;
        tripType = "RIGA";
    }
    
    // Constructor
    Trip(std::string id, Transport* v, std::string from, std::string to, std::string time, std::string type) {
        tripId = id;
        vehicle = v;
        fromCity = from;
        toCity = to;
        departureTime = time;
        tripType = type;  
    }
    
    // Calculate price for this trip
    double getPrice(int age, bool hasStudentCard, RigaTicketType ticketType) {
        if (tripType == "RIGA") {
            return vehicle->calculatePrice(age, hasStudentCard, ticketType);
        } else {
            return vehicle->calculateTrainPrice(age, hasStudentCard, fromCity, toCity);
        }
    }
    
    // Trip information
    void printInfo() {
        std::cout << tripId << ": " << vehicle->getInfo() << std::endl;
        std::cout << "   " << fromCity << " -> " << toCity;
        std::cout << ", time: " << departureTime << std::endl;
    }
};

#endif