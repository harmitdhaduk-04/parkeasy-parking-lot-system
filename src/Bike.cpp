#include "../include/Bike.h"
#include <iomanip>

Bike::Bike(const std::string& reg, const std::string& owner, int hours)
    : Vehicle(reg, owner, hours)
{
}

double Bike::calculateCharge() const
{
    return ParkingFeeCalculator::calculateBikeFee(parkingHours);
}

VehicleType Bike::getType() const
{
    return VehicleType::Bike;
}

std::string Bike::getTypeString() const
{
    return "Bike";
}

void Bike::display(std::ostream& os) const
{
    os << "--- BIKE DETAILS ---\n";
    Vehicle::display(os);
    os << "Vehicle Type   : Bike\n";
    os << "Hourly Rate    : Rs. " << ParkingFeeCalculator::getBikeHourlyRate() << " / hr\n";
    os << "Parking Charge : Rs. " << std::fixed << std::setprecision(2) << calculateCharge() << "\n";
}
