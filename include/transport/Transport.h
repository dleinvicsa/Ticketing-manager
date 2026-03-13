#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>
#include <iostream>
#include "VehicleType.h"

class Transport {
private:
    VehicleType vehicleType;
    std::string routeNumber;
    
public:
    // Constructor
    Transport(VehicleType type, std::string route) {
        vehicleType = type;
        routeNumber = route;
    }
    
    std::string getStationZone(std::string stationName) {
        // Zone A - Riga center and nearby
        if (stationName == "Riga" || stationName == "Tornakalns" || 
            stationName == "Zasulauks" || stationName == "Imanta") {
            return "A";
        }
        // Zone B - Riga suburbs
        else if (stationName == "Dubulti" || stationName == "Sloka" || 
                 stationName == "Kemeri" || stationName == "Carnikava" || 
                 stationName == "Saulkrasti") {
            return "B";
        }
        // Zone C - Medium distance cities
        else if (stationName == "Sigulda" || stationName == "Jelgava" || 
                 stationName == "Ogre" || stationName == "Cesis") {
            return "C";
        }
        // Zone D - Medium-long distance
        else if (stationName == "Tukums II" || stationName == "Madona") {
            return "D";
        }
        // Zone E - Long distance
        else if (stationName == "Aizkraukle" || stationName == "Daugavpils" || 
                 stationName == "Rezekne" || stationName == "Liepaja" || 
                 stationName == "Valmiera" || stationName == "Kraslava" || 
                 stationName == "Zilupe") {
            return "E";
        }
        else {
            return "A";  
        }
    }
    
    // PRICE CALCULATION FUNCTION - FOR RIGA CITY TRANSPORT
    double calculatePrice(int age, bool hasStudentCard, RigaTicketType ticketType) {
        
        double price = 0.0;
        
        // -----  DETERMINE BASE PRICE BY VEHICLE TYPE -----
        
        // Special case: Bus number 22 
        if (vehicleType == VehicleType::BUS && routeNumber == "22" && ticketType == RigaTicketType::BUS_22_ONLY) {
            price = 1.81;  
        }
        else {
            // All other vehicles 
            if (ticketType == RigaTicketType::SINGLE_90MIN) {
                price = 1.50;
            }
            else if (ticketType == RigaTicketType::DAILY_24H) {
                price = 5.00;
            }
            else if (ticketType == RigaTicketType::DAILY_72H) {
                price = 8.00;
            }
            else if (ticketType == RigaTicketType::DAILY_120H) {
                price = 10.00;
            }
            else if (ticketType == RigaTicketType::MONTHLY_30D) {
                price = 30.00;
            }
            else {
                price = 1.50;  // Default price
            }
        }
        
        // ----- APPLY DISCOUNTS BASED ON AGE AND STUDENT CARD -----
        
        // Children under 7 are FREE
        if (age < 7) {
            return 0.0;
        }
        
        // 65+ get 50% DISCOUNT
        if (age >= 65) {
            return price * 0.5;  // Half price
        }
        
        // Student card discount
        if (hasStudentCard) {
            if (ticketType == RigaTicketType::SINGLE_90MIN) {
                return price * 0.5;  // 50% discount for single tickets
            } else {
                return price * 0.6;  // 40% discount for passes
            }
        }
        
        // No discounts
        return price;
    }
    
    // VIVI TRAINS - WITH STATION ZONES 
    double calculateTrainPrice(int age, bool hasStudentCard, std::string fromStation, std::string toStation) {
        
        double price = 0.0;
        
        // ----- CHECK SPECIAL LONG DISTANCE ROUTES FIRST -----
        
        if ((fromStation == "Riga" && toStation == "Daugavpils") || 
            (fromStation == "Daugavpils" && toStation == "Riga")) {
            price = 8.90;
        }
        else if ((fromStation == "Riga" && toStation == "Liepaja") || 
                 (fromStation == "Liepaja" && toStation == "Riga")) {
            price = 9.10;
        }
        else if ((fromStation == "Riga" && toStation == "Valmiera") || 
                 (fromStation == "Valmiera" && toStation == "Riga")) {
            price = 5.50;
        }
        else if ((fromStation == "Riga" && toStation == "Rezekne") || 
                 (fromStation == "Rezekne" && toStation == "Riga")) {
            price = 7.20;
        }
        else {
            // ----- IF NOT SPECIAL, CALCULATE BY ZONES -----
            
            // Find zones of stations
            std::string fromZone = getStationZone(fromStation);
            std::string toZone = getStationZone(toStation);
            
            // Same zone (A-A, B-B, C-C, etc.)
            if (fromZone == toZone) {
                price = 1.50;
            }
            // A <-> B (Riga to suburbs)
            else if ((fromZone == "A" && toZone == "B") || (fromZone == "B" && toZone == "A")) {
                price = 2.30;
            }
            // A <-> C (Riga to Sigulda, Ogre, Jelgava)
            else if ((fromZone == "A" && toZone == "C") || (fromZone == "C" && toZone == "A")) {
                price = 2.80;
            }
            // A <-> D (Riga to Tukums)
            else if ((fromZone == "A" && toZone == "D") || (fromZone == "D" && toZone == "A")) {
                price = 3.30;
            }
            // A <-> E (Riga to Aizkraukle and other long distance - but not special ones)
            else if ((fromZone == "A" && toZone == "E") || (fromZone == "E" && toZone == "A")) {
                price = 3.80;
            }
            else {
                price = 2.50;  // Default price
            }
        }
        
        // ----- APPLY DISCOUNTS -----
        
        if (age < 7) {
            return 0.0;
        }
        
        if (age >= 65) {
            return price * 0.5;
        }
        
        if (hasStudentCard) {
            return price * 0.6;
        }
        
        return price;
    }
    
    // Vehicle information
    std::string getInfo() {
        std::string typeStr;
        if (vehicleType == VehicleType::BUS) typeStr = "Bus";
        else if (vehicleType == VehicleType::TRAM) typeStr = "Tram";
        else if (vehicleType == VehicleType::TROLLEY) typeStr = "Trolleybus";
        else typeStr = "Train";
        
        return typeStr + " " + routeNumber;
    }
};

#endif