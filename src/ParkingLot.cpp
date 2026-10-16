#include "../include/ParkingLot.h"
#include <iomanip>

ParkingLot::ParkingLot(int lotCapacity)
    : capacity(lotCapacity <= 0 ? 10 : lotCapacity)
{
    slots.reserve(capacity);
    for (int i = 0; i < capacity; ++i)
    {
        slots.emplace_back(i + 1);
    }
}

int ParkingLot::findSlotIndex(const std::string& regNumber) const
{
    for (size_t i = 0; i < slots.size(); ++i)
    {
        if (slots[i].isOccupied && slots[i].vehicle &&
            slots[i].vehicle->getRegistrationNumber() == regNumber)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int ParkingLot::findNextAvailableSlot() const
{
    for (const auto& slot : slots)
    {
        if (!slot.isOccupied)
        {
            return slot.slotNumber;
        }
    }
    return -1;
}

ParkingTicket ParkingLot::parkVehicle(std::unique_ptr<Vehicle> vehicle)
{
    if (!vehicle)
    {
        throw std::invalid_argument("Cannot park a null vehicle pointer.");
    }

    if (vehicle->getParkingHours() <= 0)
    {
        throw InvalidDurationException();
    }

    if (findSlotIndex(vehicle->getRegistrationNumber()) != -1)
    {
        throw DuplicateVehicleException(vehicle->getRegistrationNumber());
    }

    int freeSlot = findNextAvailableSlot();
    if (freeSlot == -1)
    {
        throw ParkingLotFullException();
    }

    int slotIdx = freeSlot - 1;

    // Generate ticket
    ParkingTicket ticket(vehicle->getRegistrationNumber(),
                          vehicle->getOwnerName(),
                          vehicle->getType(),
                          freeSlot,
                          vehicle->getParkingHours());

    // Strong exception guarantee: slot is updated only after all validations & ticket creation
    slots[slotIdx].vehicle = std::move(vehicle);
    slots[slotIdx].ticket = std::make_unique<ParkingTicket>(ticket);
    slots[slotIdx].isOccupied = true;

    return ticket;
}

ParkingTicket ParkingLot::parkVehicle(Vehicle* vehicle)
{
    if (!vehicle)
    {
        throw std::invalid_argument("Cannot park a null vehicle pointer.");
    }
    return parkVehicle(std::unique_ptr<Vehicle>(vehicle));
}

double ParkingLot::removeVehicle(const std::string& regNumber)
{
    return removeVehicle(regNumber, static_cast<ParkingTicket*>(nullptr));
}

double ParkingLot::removeVehicle(const std::string& regNumber, ParkingTicket* outTicket)
{
    int idx = findSlotIndex(regNumber);
    if (idx == -1)
    {
        throw VehicleNotFoundException(regNumber);
    }

    double fee = slots[idx].vehicle->calculateCharge();

    if (outTicket && slots[idx].ticket)
    {
        *outTicket = *(slots[idx].ticket);
    }

    // Release slot resources
    slots[idx].vehicle.reset();
    slots[idx].ticket.reset();
    slots[idx].isOccupied = false;

    return fee;
}

double ParkingLot::removeVehicle(const std::string& regNumber, ParkingTicket& outTicket)
{
    return removeVehicle(regNumber, &outTicket);
}

void ParkingLot::restoreVehicle(std::unique_ptr<Vehicle> vehicle, int slotNumber, const ParkingTicket& ticket)
{
    if (!vehicle)
    {
        throw std::invalid_argument("Cannot restore a null vehicle pointer.");
    }

    if (slotNumber < 1 || slotNumber > capacity)
    {
        throw InvalidSlotException("Slot number " + std::to_string(slotNumber) + " is outside capacity [1, " + std::to_string(capacity) + "]");
    }

    int idx = slotNumber - 1;
    if (slots[idx].isOccupied)
    {
        throw InvalidSlotException("Slot number " + std::to_string(slotNumber) + " is already occupied during restore.");
    }

    if (findSlotIndex(vehicle->getRegistrationNumber()) != -1)
    {
        throw DuplicateVehicleException(vehicle->getRegistrationNumber());
    }

    slots[idx].vehicle = std::move(vehicle);
    slots[idx].ticket = std::make_unique<ParkingTicket>(ticket);
    slots[idx].isOccupied = true;
}

// Function overloading: Search vehicle by registration number
const Vehicle* ParkingLot::searchVehicle(const std::string& regNumber) const
{
    int idx = findSlotIndex(regNumber);
    if (idx != -1)
    {
        return slots[idx].vehicle.get();
    }
    return nullptr;
}

// Function overloading: Search vehicle by assigned slot number
const Vehicle* ParkingLot::searchVehicle(int slotNumber) const
{
    if (slotNumber >= 1 && slotNumber <= capacity)
    {
        int idx = slotNumber - 1;
        if (slots[idx].isOccupied)
        {
            return slots[idx].vehicle.get();
        }
    }
    return nullptr;
}

const ParkingTicket* ParkingLot::getTicket(const std::string& regNumber) const
{
    int idx = findSlotIndex(regNumber);
    if (idx != -1)
    {
        return slots[idx].ticket.get();
    }
    return nullptr;
}

const ParkingTicket* ParkingLot::getTicketBySlot(int slotNumber) const
{
    if (slotNumber >= 1 && slotNumber <= capacity)
    {
        int idx = slotNumber - 1;
        if (slots[idx].isOccupied)
        {
            return slots[idx].ticket.get();
        }
    }
    return nullptr;
}

// Function overloading: Display to standard output
void ParkingLot::displayVehicles() const
{
    displayVehicles(std::cout);
}

// Function overloading: Display to arbitrary output stream
void ParkingLot::displayVehicles(std::ostream& os) const
{
    int occupied = getOccupiedCount();
    if (occupied == 0)
    {
        os << "\n[Notice] No vehicles are currently parked in the lot.\n";
        return;
    }

    os << "\n=======================================================\n";
    os << "               CURRENTLY PARKED VEHICLES               \n";
    os << "=======================================================\n";
    os << " Occupied Spaces: " << occupied << " / " << capacity << "\n";
    os << "-------------------------------------------------------\n";

    for (const auto& slot : slots)
    {
        if (slot.isOccupied && slot.vehicle)
        {
            os << "[Parking Space: Slot #" << slot.slotNumber << "]\n";
            if (slot.ticket)
            {
                os << "Ticket ID      : " << slot.ticket->getTicketId() << "\n";
            }
            // Polymorphic dispatch calling Car::display or Bike::display
            slot.vehicle->display(os);
            os << "-------------------------------------------------------\n";
        }
    }
}

std::vector<int> ParkingLot::getAvailableSlots() const
{
    std::vector<int> available;
    for (const auto& slot : slots)
    {
        if (!slot.isOccupied)
        {
            available.push_back(slot.slotNumber);
        }
    }
    return available;
}

std::vector<int> ParkingLot::getOccupiedSlots() const
{
    std::vector<int> occupied;
    for (const auto& slot : slots)
    {
        if (slot.isOccupied)
        {
            occupied.push_back(slot.slotNumber);
        }
    }
    return occupied;
}

int ParkingLot::getCapacity() const
{
    return capacity;
}

int ParkingLot::getOccupiedCount() const
{
    int count = 0;
    for (const auto& slot : slots)
    {
        if (slot.isOccupied)
        {
            count++;
        }
    }
    return count;
}

int ParkingLot::getAvailableCount() const
{
    return capacity - getOccupiedCount();
}

void ParkingLot::clear()
{
    for (auto& slot : slots)
    {
        slot.vehicle.reset();
        slot.ticket.reset();
        slot.isOccupied = false;
    }
}

const std::vector<ParkingSlot>& ParkingLot::getSlots() const
{
    return slots;
}
