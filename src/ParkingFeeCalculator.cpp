#include "../include/ParkingFeeCalculator.h"
#include <algorithm>

double ParkingFeeCalculator::getCarHourlyRate()
{
    return CAR_HOURLY_RATE;
}

double ParkingFeeCalculator::getBikeHourlyRate()
{
    return BIKE_HOURLY_RATE;
}

double ParkingFeeCalculator::calculateCarFee(int hours)
{
    if (hours <= 0) return 0.0;
    return hours * CAR_HOURLY_RATE;
}

double ParkingFeeCalculator::calculateBikeFee(int hours)
{
    if (hours <= 0) return 0.0;
    return hours * BIKE_HOURLY_RATE;
}

double ParkingFeeCalculator::calculateFee(VehicleType type, int hours)
{
    switch (type)
    {
        case VehicleType::Car:
            return calculateCarFee(hours);
        case VehicleType::Bike:
            return calculateBikeFee(hours);
        default:
            return 0.0;
    }
}

std::string ParkingFeeCalculator::vehicleTypeToString(VehicleType type)
{
    switch (type)
    {
        case VehicleType::Car:
            return "Car";
        case VehicleType::Bike:
            return "Bike";
        default:
            return "Unknown";
    }
}

VehicleType ParkingFeeCalculator::stringToVehicleType(const std::string& str)
{
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "car") return VehicleType::Car;
    if (lower == "bike") return VehicleType::Bike;
    return VehicleType::Unknown;
}
