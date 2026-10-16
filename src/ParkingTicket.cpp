#include "../include/ParkingTicket.h"
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>

int ParkingTicket::ticketCounter = 1000;

ParkingTicket::ParkingTicket()
    : ticketId(""),
      registrationNumber(""),
      ownerName(""),
      vehicleType(VehicleType::Unknown),
      assignedSlot(0),
      parkingHours(0),
      estimatedFee(0.0),
      entryTime("")
{
}

ParkingTicket::ParkingTicket(const std::string& reg,
                             const std::string& owner,
                             VehicleType type,
                             int slot,
                             int hours,
                             const std::string& timestamp,
                             const std::string& customId)
    : registrationNumber(reg),
      ownerName(owner),
      vehicleType(type),
      assignedSlot(slot),
      parkingHours(hours)
{
    ticketId = customId.empty() ? generateTicketId() : customId;
    estimatedFee = ParkingFeeCalculator::calculateFee(type, hours);

    if (timestamp.empty())
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* tm_now = std::localtime(&now_c);
        char buffer[32];
        if (tm_now)
        {
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_now);
            entryTime = buffer;
        }
        else
        {
            entryTime = "N/A";
        }
    }
    else
    {
        entryTime = timestamp;
    }
}

std::string ParkingTicket::getTicketId() const
{
    return ticketId;
}

std::string ParkingTicket::getRegistrationNumber() const
{
    return registrationNumber;
}

std::string ParkingTicket::getOwnerName() const
{
    return ownerName;
}

VehicleType ParkingTicket::getVehicleType() const
{
    return vehicleType;
}

int ParkingTicket::getAssignedSlot() const
{
    return assignedSlot;
}

int ParkingTicket::getParkingHours() const
{
    return parkingHours;
}

double ParkingTicket::getEstimatedFee() const
{
    return estimatedFee;
}

std::string ParkingTicket::getEntryTime() const
{
    return entryTime;
}

void ParkingTicket::displayTicket(std::ostream& os) const
{
    os << "\n========================================\n"
       << "            PARKING TICKET              \n"
       << "========================================\n"
       << " Ticket ID       : " << ticketId << "\n"
       << " Assigned Space  : Slot #" << assignedSlot << "\n"
       << " Registration No : " << registrationNumber << "\n"
       << " Owner Name      : " << ownerName << "\n"
       << " Vehicle Type    : " << ParkingFeeCalculator::vehicleTypeToString(vehicleType) << "\n"
       << " Parking Duration: " << parkingHours << " hrs\n"
       << " Estimated Fee   : Rs. " << std::fixed << std::setprecision(2) << estimatedFee << "\n"
       << " Entry Time      : " << entryTime << "\n"
       << "========================================\n";
}

std::string ParkingTicket::generateTicketId()
{
    std::ostringstream oss;
    oss << "TKT-" << (++ticketCounter);
    return oss.str();
}

void ParkingTicket::resetTicketCounter(int start)
{
    ticketCounter = start;
}
