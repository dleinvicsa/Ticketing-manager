#include <iostream>
#include <vector>
#include "Transport.h"
#include "Trip.h"

int main() {
    std::cout << "========================================\n";
    std::cout << "LATVIAN TRANSPORT SYSTEM\n";
    std::cout << "========================================\n\n";
    
    // ----- CREATE VEHICLES -----
    std::cout << "1. CREATING VEHICLES...\n";
    
    Transport bus22(VehicleType::BUS, "22");      // Bus number 22
    Transport bus11(VehicleType::BUS, "11");      // Bus number 11
    Transport tram5(VehicleType::TRAM, "5");      // Tram number 5
    Transport trolley9(VehicleType::TROLLEY, "9"); // Trolleybus number 9
    Transport train(VehicleType::TRAIN, "V-13");   // Vivi train
    
    std::cout << "   ✓ 4 vehicles created\n\n";
    
    // ----- CREATE TRIPS -----
    std::cout << "2. CREATING TRIPS...\n";
    
    std::vector<Trip> trips;
    
    // Riga trips
    trips.push_back(Trip("R001", &bus22, "Riga", "Riga", "08:30"));
    trips.push_back(Trip("R002", &bus11, "Riga", "Riga", "10:15"));
    trips.push_back(Trip("R003", &tram5, "Riga", "Riga", "12:00"));
    trips.push_back(Trip("R004", &trolley9, "Riga", "Riga", "15:30"));
    
    // Vivi train trips 
    trips.push_back(Trip("V001", &train, "Riga", "Imanta", "09:00", "VIVI"));
    trips.push_back(Trip("V002", &train, "Riga", "Dubulti", "09:30", "VIVI"));
    trips.push_back(Trip("V003", &train, "Riga", "Sigulda", "11:30", "VIVI"));
    trips.push_back(Trip("V004", &train, "Riga", "Tukums II", "13:15", "VIVI"));
    trips.push_back(Trip("V005", &train, "Riga", "Aizkraukle", "14:45", "VIVI"));
    trips.push_back(Trip("V006", &train, "Riga", "Daugavpils", "07:45", "VIVI"));
    trips.push_back(Trip("V007", &train, "Riga", "Liepaja", "08:30", "VIVI"));
    trips.push_back(Trip("V008", &train, "Riga", "Valmiera", "09:45", "VIVI"));
    trips.push_back(Trip("V009", &train, "Riga", "Rezekne", "10:30", "VIVI"));
    trips.push_back(Trip("V010", &train, "Sigulda", "Riga", "16:20", "VIVI"));
    
    std::cout << "   ✓ " << trips.size() << " trips created\n\n";
    
    // ----- LIST ALL TRIPS -----
    std::cout << "========================================\n";
    std::cout << "ALL TRIPS:\n";
    std::cout << "========================================\n";
    
    for (size_t i = 0; i < trips.size(); i++) {
        trips[i].printInfo();
    }
    
    // ----- PRICE CALCULATION EXAMPLES -----
    std::cout << "\n========================================\n";
    std::cout << "PRICE CALCULATION EXAMPLES:\n";
    std::cout << "========================================\n\n";
    
    // Bus number 22 
    std::cout << "1. BUS NUMBER 22 (buy from driver):\n";
    std::cout << "   Adult (30 years): €" 
         << bus22.calculatePrice(30, false, RigaTicketType::BUS_22_ONLY) << std::endl;
    std::cout << "   Student (20 years): €" 
         << bus22.calculatePrice(20, true, RigaTicketType::BUS_22_ONLY) << std::endl;
    std::cout << "   Senior (70 years): €" 
         << bus22.calculatePrice(70, false, RigaTicketType::BUS_22_ONLY) << std::endl;
    std::cout << "   Child (5 years): €" 
         << bus22.calculatePrice(5, false, RigaTicketType::BUS_22_ONLY) << std::endl;
    
    // Bus number 11 (90 min ticket)
    std::cout << "\n2. BUS NUMBER 11 (90 min ticket):\n";
    std::cout << "   Adult (30 years): €" 
         << bus11.calculatePrice(30, false, RigaTicketType::SINGLE_90MIN) << std::endl;
    std::cout << "   Student (20 years): €" 
         << bus11.calculatePrice(20, true, RigaTicketType::SINGLE_90MIN) << std::endl;
    std::cout << "   Senior (70 years): €" 
         << bus11.calculatePrice(70, false, RigaTicketType::SINGLE_90MIN) << std::endl;
    
    // Tram (24 hour)
    std::cout << "\n3. TRAM NUMBER 5 (24 hour ticket):\n";
    std::cout << "   Adult (30 years): €" 
         << tram5.calculatePrice(30, false, RigaTicketType::DAILY_24H) << std::endl;
    std::cout << "   Student (20 years): €" 
         << tram5.calculatePrice(20, true, RigaTicketType::DAILY_24H) << std::endl;
    
    // Trolleybus (30 day pass)
    std::cout << "\n4. TROLLEYBUS NUMBER 9 (30 day pass):\n";
    std::cout << "   Adult (30 years): €" 
         << trolley9.calculatePrice(30, false, RigaTicketType::MONTHLY_30D) << std::endl;
    std::cout << "   Student (20 years): €" 
         << trolley9.calculatePrice(20, true, RigaTicketType::MONTHLY_30D) << std::endl;
    
    // Trains with stations (testing special routes)
    std::cout << "\n5. VIVI TRAINS (special long distance routes):\n";
    std::cout << "   Riga - Daugavpils: €" 
         << train.calculateTrainPrice(30, false, "Riga", "Daugavpils") << std::endl;
    std::cout << "   Riga - Liepaja: €" 
         << train.calculateTrainPrice(30, false, "Riga", "Liepaja") << std::endl;
    std::cout << "   Riga - Valmiera: €" 
         << train.calculateTrainPrice(30, false, "Riga", "Valmiera") << std::endl;
    std::cout << "   Riga - Rezekne: €" 
         << train.calculateTrainPrice(30, false, "Riga", "Rezekne") << std::endl;
    
    // Trains with zones
    std::cout << "\n6. VIVI TRAINS (zone based):\n";
    std::cout << "   Riga - Imanta (A zone): €" 
         << train.calculateTrainPrice(30, false, "Riga", "Imanta") << std::endl;
    std::cout << "   Riga - Dubulti (A to B): €" 
         << train.calculateTrainPrice(30, false, "Riga", "Dubulti") << std::endl;
    std::cout << "   Riga - Sigulda (A to C): €" 
         << train.calculateTrainPrice(30, false, "Riga", "Sigulda") << std::endl;
    std::cout << "   Riga - Tukums II (A to D): €" 
         << train.calculateTrainPrice(30, false, "Riga", "Tukums II") << std::endl;
    std::cout << "   Riga - Aizkraukle (A to E): €" 
         << train.calculateTrainPrice(30, false, "Riga", "Aizkraukle") << std::endl;
    
    // Different ages comparison
    std::cout << "\n7. DIFFERENT AGES (Bus 11, 90 min ticket):\n";
    int ages[] = {5, 15, 25, 35, 45, 55, 65, 75};
    int arraySize = sizeof(ages) / sizeof(ages[0]);
    
    for (int i = 0; i < arraySize; i++) {
        int age = ages[i];
        std::cout << "   Age " << age << ": €" 
             << bus11.calculatePrice(age, false, RigaTicketType::SINGLE_90MIN) << std::endl;
    }
    
    return 0;
}