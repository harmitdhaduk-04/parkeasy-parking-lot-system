#ifndef BIKE_H
#define BIKE_H

#include "Vehicle.h"

/**
 * @brief Bike class derived from Vehicle.
 * Demonstrates Single Inheritance and Runtime Polymorphism.
 */
class Bike : public Vehicle
{
public:
    Bike(const std::string& reg = "", const std::string& owner = "", int hours = 0);
    virtual ~Bike() override = default;

    // Overridden methods
    double calculateCharge() const override;
    VehicleType getType() const override;
    std::string getTypeString() const override;
    void display(std::ostream& os = std::cout) const override;
};

#endif // BIKE_H
