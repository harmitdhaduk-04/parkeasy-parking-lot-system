#ifndef PARKING_TICKET_H
#define PARKING_TICKET_H

#include <iostream>
#include <string>
#include "ParkingFeeCalculator.h"

/**
 * @brief Class representing a parking ticket issued to a vehicle upon parking.
 *
 * Encapsulates ticket identifier, vehicle details, assigned slot number,
 * parking duration, entry timestamp, and fee estimates.
 */
class ParkingTicket
{
private:
    std::string ticketId;
    std::string registrationNumber;
    std::string ownerName;
    VehicleType vehicleType;
    int assignedSlot;
    int parkingHours;
    double estimatedFee;
    std::string entryTime;

    static int ticketCounter;

public:
    ParkingTicket();
    ParkingTicket(const std::string& reg,
                  const std::string& owner,
                  VehicleType type,
                  int slot,
                  int hours,
                  const std::string& timestamp = "",
                  const std::string& customId = "");

    // Getters
    std::string getTicketId() const;
    std::string getRegistrationNumber() const;
    std::string getOwnerName() const;
    VehicleType getVehicleType() const;
    int getAssignedSlot() const;
    int getParkingHours() const;
    double getEstimatedFee() const;
    std::string getEntryTime() const;

    // Display
    void displayTicket(std::ostream& os = std::cout) const;

    // Helper to generate unique ticket ID
    static std::string generateTicketId();
    static void resetTicketCounter(int start = 1000);
};

#endif // PARKING_TICKET_H
