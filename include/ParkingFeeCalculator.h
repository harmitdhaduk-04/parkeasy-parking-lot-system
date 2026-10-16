#ifndef PARKING_FEE_CALCULATOR_H
#define PARKING_FEE_CALCULATOR_H

#include <string>
#include <iostream>

// Enum representing supported vehicle types
enum class VehicleType
{
    Car,
    Bike,
    Unknown
};

inline std::ostream& operator<<(std::ostream& os, VehicleType type)
{
    switch (type)
    {
        case VehicleType::Car: return os << "Car";
        case VehicleType::Bike: return os << "Bike";
        default: return os << "Unknown";
    }
}

/**
 * @brief Centralized fee calculation class.
 *
 * Implements encapsulation and separation of concerns by maintaining
 * parking fee hourly rates and calculating charges without hardcoding rates elsewhere.
 */
class ParkingFeeCalculator
{
private:
    // Fixed hourly rates in Rupees
    static constexpr double CAR_HOURLY_RATE = 50.0;
    static constexpr double BIKE_HOURLY_RATE = 20.0;

public:
    static double getCarHourlyRate();
    static double getBikeHourlyRate();

    static double calculateCarFee(int hours);
    static double calculateBikeFee(int hours);
    static double calculateFee(VehicleType type, int hours);

    static std::string vehicleTypeToString(VehicleType type);
    static VehicleType stringToVehicleType(const std::string& str);
};

#endif // PARKING_FEE_CALCULATOR_H
