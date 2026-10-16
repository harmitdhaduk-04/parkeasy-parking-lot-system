#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>
#include "ParkingFeeCalculator.h"

// Forward declaration of friend functions
class Vehicle;
void showTotalVehicles();
void showVehicleDetails(const Vehicle& v);

// ================= TEMPLATE FUNCTION =================
/**
 * @brief Reusable template function for displaying any message type.
 * Demonstrates C++ Templates (Generic Programming).
 */
template <typename T>
void showMessage(const T& message)
{
    std::cout << message << std::endl;
}

/**
 * @brief Abstract Base Class representing a generic vehicle in the parking system.
 *
 * Demonstrates:
 * - Encapsulation (protected data members, public interface)
 * - Abstraction (pure virtual functions calculateCharge and getType)
 * - Static members (totalVehicles counter)
 * - Operator overloading (operator==, operator!=)
 * - Function overloading (searchVehicle)
 * - Friend functions (showTotalVehicles, showVehicleDetails)
 */
class Vehicle
{
protected:
    std::string registrationNumber;
    std::string ownerName;
    int parkingHours;

public:
    // Static member tracking total Vehicle instances created in memory
    static int totalVehicles;

    // Parameterized constructor with default values
    Vehicle(const std::string& reg = "", const std::string& owner = "", int hours = 0);

    // Virtual destructor ensuring correct cleanup in derived classes
    virtual ~Vehicle();

    // Pure virtual functions (Abstraction)
    virtual double calculateCharge() const = 0;
    virtual VehicleType getType() const = 0;
    virtual std::string getTypeString() const = 0;

    // Polymorphic display method
    virtual void display(std::ostream& os = std::cout) const;

    // Getters (Encapsulation)
    std::string getRegistrationNumber() const;
    std::string getOwnerName() const;
    int getParkingHours() const;

    // Setter for parking hours
    void setParkingHours(int hours);

    // Function overloading (Demonstrating Compile-Time Polymorphism)
    void searchVehicle(const std::string& queryReg) const;
    void searchVehicle() const;
    bool matchesRegistration(const std::string& reg) const;

    // Operator overloading
    bool operator==(const Vehicle& other) const;
    bool operator!=(const Vehicle& other) const;

    // Friend functions (have access to protected/private members)
    friend void showTotalVehicles();
    friend void showVehicleDetails(const Vehicle& v);
};

#endif // VEHICLE_H
