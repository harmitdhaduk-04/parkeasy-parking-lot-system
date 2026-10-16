#ifndef PARKING_LOT_H
#define PARKING_LOT_H

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "Vehicle.h"
#include "Car.h"
#include "Bike.h"
#include "ParkingTicket.h"
#include <stdexcept>

// ================= CUSTOM PARKING EXCEPTIONS =================

class ParkingException : public std::runtime_error
{
public:
    explicit ParkingException(const std::string& message)
        : std::runtime_error(message) {}
};

class ParkingLotFullException : public ParkingException
{
public:
    ParkingLotFullException()
        : ParkingException("Parking Lot is full! No spaces available.") {}
};

class DuplicateVehicleException : public ParkingException
{
public:
    explicit DuplicateVehicleException(const std::string& reg)
        : ParkingException("Duplicate vehicle registration: " + reg + " is already parked.") {}
};

class VehicleNotFoundException : public ParkingException
{
public:
    explicit VehicleNotFoundException(const std::string& query)
        : ParkingException("Vehicle not found: " + query) {}
};

class InvalidDurationException : public ParkingException
{
public:
    InvalidDurationException()
        : ParkingException("Invalid parking duration! Parking hours must be greater than 0.") {}
};

class InvalidVehicleTypeException : public ParkingException
{
public:
    explicit InvalidVehicleTypeException(const std::string& details)
        : ParkingException("Invalid vehicle type: " + details) {}
};

class InvalidSlotException : public ParkingException
{
public:
    explicit InvalidSlotException(const std::string& details)
        : ParkingException("Invalid slot: " + details) {}
};

/**
 * @brief Represents a dedicated parking slot within the parking lot.
 */
struct ParkingSlot
{
    int slotNumber;
    bool isOccupied;
    std::unique_ptr<Vehicle> vehicle;
    std::unique_ptr<ParkingTicket> ticket;

    explicit ParkingSlot(int number = 0)
        : slotNumber(number), isOccupied(false), vehicle(nullptr), ticket(nullptr) {}

    // Disable copy, enable move
    ParkingSlot(const ParkingSlot&) = delete;
    ParkingSlot& operator=(const ParkingSlot&) = delete;
    ParkingSlot(ParkingSlot&&) noexcept = default;
    ParkingSlot& operator=(ParkingSlot&&) noexcept = default;
};

/**
 * @brief Manages parking slots, vehicle operations, and business policies.
 *
 * Demonstrates:
 * - RAII and smart pointer management (std::unique_ptr)
 * - Encapsulation of parking capacity and slot allocation
 * - Polymorphism across stored Vehicle pointers
 * - Function overloading (searchVehicle by registration string and by slot int)
 * - Exception safety (Strong exception guarantee on parking/removal)
 */
class ParkingLot
{
private:
    int capacity;
    std::vector<ParkingSlot> slots;

    // Helper to find slot index for a registration number (-1 if not found)
    int findSlotIndex(const std::string& regNumber) const;

    // Helper to find the lowest available free slot number (1-based, or -1 if full)
    int findNextAvailableSlot() const;

public:
    // Default capacity is 10
    explicit ParkingLot(int lotCapacity = 10);
    ~ParkingLot() = default;

    // Non-copyable due to unique_ptr, movable
    ParkingLot(const ParkingLot&) = delete;
    ParkingLot& operator=(const ParkingLot&) = delete;
    ParkingLot(ParkingLot&&) noexcept = default;
    ParkingLot& operator=(ParkingLot&&) noexcept = default;

    // Core parking operations (accepts both unique_ptr<Vehicle> and raw pointer Vehicle*)
    ParkingTicket parkVehicle(std::unique_ptr<Vehicle> vehicle);
    ParkingTicket parkVehicle(Vehicle* vehicle);

    // Overloaded removal operations (compatible with no extra args, pointer, or reference)
    double removeVehicle(const std::string& regNumber);
    double removeVehicle(const std::string& regNumber, ParkingTicket* outTicket);
    double removeVehicle(const std::string& regNumber, ParkingTicket& outTicket);

    // Direct restoration for persistence loader
    void restoreVehicle(std::unique_ptr<Vehicle> vehicle, int slotNumber, const ParkingTicket& ticket);

    // Function overloading: Search by registration number vs search by slot number
    const Vehicle* searchVehicle(const std::string& regNumber) const;
    const Vehicle* searchVehicle(int slotNumber) const;

    // Ticket access
    const ParkingTicket* getTicket(const std::string& regNumber) const;
    const ParkingTicket* getTicketBySlot(int slotNumber) const;

    // Function overloading: Display to stdout vs arbitrary stream
    void displayVehicles() const;
    void displayVehicles(std::ostream& os) const;

    // Slot queries
    std::vector<int> getAvailableSlots() const;
    std::vector<int> getOccupiedSlots() const;
    int getCapacity() const;
    int getOccupiedCount() const;
    int getAvailableCount() const;

    // Clear all slots
    void clear();

    // Read-only slot inspector (used by persistence manager)
    const std::vector<ParkingSlot>& getSlots() const;
};

#endif // PARKING_LOT_H
