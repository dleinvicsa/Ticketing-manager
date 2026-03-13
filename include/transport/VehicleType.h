#ifndef VEHICLETYPE_H
#define VEHICLETYPE_H

// Vehicle types
enum class VehicleType {
    BUS,        // Bus
    TRAM,       // Tram
    TROLLEY,    // Trolleybus
    TRAIN       // Train 
};

// Riga ticket types
enum class RigaTicketType {
    SINGLE_90MIN,   // 90 minute ticket
    DAILY_24H,      // 24 hour ticket
    DAILY_72H,      // 72 hour ticket
    DAILY_120H,     // 120 hour ticket
    MONTHLY_30D,    // 30 day monthly pass
    BUS_22_ONLY     // Only for bus number 22 
};

#endif