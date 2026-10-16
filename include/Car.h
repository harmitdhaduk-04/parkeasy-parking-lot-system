#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"

/**
 * @brief Car class derived from Vehicle.
 * Demonstrates Single Inheritance and Runtime Polymorphism.
 */
class Car : public Vehicle
{
public:
    Car(const std::string& reg = "", const std::string& owner = "", int hours = 0);
    virtual ~Car() override = default;

    // Overridden methods
    double calculateCharge() const override;
    VehicleType getType() const override;
    std::string getTypeString() const override;
    void display(std::ostream& os = std::cout) const override;
};

#endif // CAR_H
