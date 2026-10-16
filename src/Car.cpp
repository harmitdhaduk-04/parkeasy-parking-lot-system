#include "../include/Car.h"
#include <iomanip>

Car::Car(const std::string& reg, const std::string& owner, int hours)
    : Vehicle(reg, owner, hours)
{
}

double Car::calculateCharge() const
{
    return ParkingFeeCalculator::calculateCarFee(parkingHours);
}

VehicleType Car::getType() const
{
    return VehicleType::Car;
}

std::string Car::getTypeString() const
{
    return "Car";
}

void Car::display(std::ostream& os) const
{
    os << "--- CAR DETAILS ---\n";
    Vehicle::display(os);
    os << "Vehicle Type   : Car\n";
    os << "Hourly Rate    : Rs. " << ParkingFeeCalculator::getCarHourlyRate() << " / hr\n";
    os << "Parking Charge : Rs. " << std::fixed << std::setprecision(2) << calculateCharge() << "\n";
}
