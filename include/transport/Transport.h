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
    Transport(VehicleType type, std::string route) {
        vehicleType = type;
        routeNumber = route;
    }
    std::string getStationZone(std::string stationName) {
        if (stationName == "Riga" || stationName == "Tornakalns" || 
            stationName == "Zasulauks" || stationName == "Imanta") {
            return "A";
        }
        else if (stationName == "Dubulti" || stationName == "Sloka" || 
                 stationName == "Kemeri" || stationName == "Carnikava" || 
                 stationName == "Saulkrasti") {
            return "B";
        }
        else if (stationName == "Sigulda" || stationName == "Jelgava" || 
                 stationName == "Ogre" || stationName == "Cesis") {
            return "C";
        }
        else if (stationName == "Tukums II" || stationName == "Madona") {
            return "D";
        }
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
    double calculatePrice(bool hasStudentCard, RigaTicketType ticketType) {
        double price = 0.0;
        if (vehicleType == VehicleType::BUS && routeNumber == "22" && ticketType == RigaTicketType::BUS_22_ONLY) {
            price = 1.81;  
        }
        else {
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
                price = 1.50;  
            }
        }
        if (hasStudentCard) {
            if (ticketType == RigaTicketType::SINGLE_90MIN) {
                return price * 0.5;  
            } else {
                return price * 0.6;  
            }
        }
        return price;
    }
    double calculateTrainPrice(bool hasStudentCard, std::string fromStation, std::string toStation) {
        double price = 0.0;
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
            std::string fromZone = getStationZone(fromStation);
            std::string toZone = getStationZone(toStation);
            if (fromZone == toZone) {
                price = 1.50;
            }
            else if ((fromZone == "A" && toZone == "B") || (fromZone == "B" && toZone == "A")) {
                price = 2.30;
            }
            else if ((fromZone == "A" && toZone == "C") || (fromZone == "C" && toZone == "A")) {
                price = 2.80;
            }
            else if ((fromZone == "A" && toZone == "D") || (fromZone == "D" && toZone == "A")) {
                price = 3.30;
            }
            else if ((fromZone == "A" && toZone == "E") || (fromZone == "E" && toZone == "A")) {
                price = 3.80;
            }
            else {
                price = 2.50;  
            }
        }
        if (hasStudentCard) {
            return price * 0.6;
        }
        return price;
    }
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