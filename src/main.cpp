#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include <iomanip>
#include "../include/ParkingLot.h"
#include "../include/Car.h"
#include "../include/Bike.h"
#include "../include/ParkingFeeCalculator.h"
#include "../include/FileManager.h"

// Constant file path for persistence
const std::string DATA_FILE_PATH = "data/parking.txt";

// Helper for safe integer input
int getValidInteger(const std::string& prompt, int minVal = 0, int maxVal = 100000)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            if (value >= minVal && value <= maxVal)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cout << "[Input Error] Please enter a value between "
                      << minVal << " and " << maxVal << ".\n";
        }
        else
        {
            std::cout << "[Input Error] Invalid input. Please enter a valid number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Helper for non-empty string input
std::string getValidString(const std::string& prompt)
{
    std::string value;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, value);

        // Trim leading and trailing whitespace
        size_t first = value.find_first_not_of(" \t\r\n");
        if (first != std::string::npos)
        {
            size_t last = value.find_last_not_of(" \t\r\n");
            value = value.substr(first, (last - first + 1));
            return value;
        }
        std::cout << "[Input Error] Input cannot be blank. Please try again.\n";
    }
}

void handleParkVehicle(ParkingLot& parkingLot)
{
    std::cout << "\n--- PARK A VEHICLE ---\n";
    std::cout << "Select Vehicle Type:\n";
    std::cout << "1. Car (Rate: Rs. " << ParkingFeeCalculator::getCarHourlyRate() << " / hr)\n";
    std::cout << "2. Bike (Rate: Rs. " << ParkingFeeCalculator::getBikeHourlyRate() << " / hr)\n";
    int typeChoice = getValidInteger("Enter choice (1-2): ", 1, 2);

    std::string regNumber = getValidString("Enter Vehicle Registration Number: ");
    std::string ownerName = getValidString("Enter Owner Name: ");
    int hours = getValidInteger("Enter Estimated Parking Duration (hours): ", 1, 720);

    std::unique_ptr<Vehicle> vehicle;
    if (typeChoice == 1)
    {
        vehicle = std::make_unique<Car>(regNumber, ownerName, hours);
    }
    else
    {
        vehicle = std::make_unique<Bike>(regNumber, ownerName, hours);
    }

    try
    {
        ParkingTicket ticket = parkingLot.parkVehicle(std::move(vehicle));
        std::cout << "\n[Success] Vehicle successfully parked!\n";
        ticket.displayTicket(std::cout);
    }
    catch (const ParkingException& ex)
    {
        std::cout << "\n[Parking Error] " << ex.what() << "\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "\n[Unexpected Error] " << ex.what() << "\n";
    }
}

void handleSearchVehicle(const ParkingLot& parkingLot)
{
    std::cout << "\n--- SEARCH VEHICLE ---\n";
    std::cout << "Search By:\n";
    std::cout << "1. Registration Number\n";
    std::cout << "2. Parking Space (Slot Number)\n";
    int choice = getValidInteger("Enter choice (1-2): ", 1, 2);

    const Vehicle* vehicle = nullptr;
    const ParkingTicket* ticket = nullptr;

    if (choice == 1)
    {
        std::string reg = getValidString("Enter Vehicle Registration Number: ");
        // Demonstrating overloaded searchVehicle(const std::string&)
        vehicle = parkingLot.searchVehicle(reg);
        ticket = parkingLot.getTicket(reg);
    }
    else
    {
        int slot = getValidInteger("Enter Parking Slot Number (1-10): ", 1, parkingLot.getCapacity());
        // Demonstrating overloaded searchVehicle(int)
        vehicle = parkingLot.searchVehicle(slot);
        ticket = parkingLot.getTicketBySlot(slot);
    }

    if (vehicle != nullptr)
    {
        std::cout << "\n========================================\n";
        std::cout << "             VEHICLE FOUND              \n";
        std::cout << "========================================\n";
        if (ticket)
        {
            std::cout << "Parking Space  : Slot #" << ticket->getAssignedSlot() << "\n";
            std::cout << "Ticket ID      : " << ticket->getTicketId() << "\n";
        }
        vehicle->display(std::cout);
        std::cout << "========================================\n";
    }
    else
    {
        std::cout << "\n[Not Found] No vehicle matches your search criteria.\n";
    }
}

void handleRemoveVehicle(ParkingLot& parkingLot)
{
    std::cout << "\n--- REMOVE VEHICLE & BILLING ---\n";
    if (parkingLot.getOccupiedCount() == 0)
    {
        std::cout << "[Notice] Parking lot is currently empty. No vehicles to remove.\n";
        return;
    }

    std::string reg = getValidString("Enter Vehicle Registration Number to remove: ");
    ParkingTicket ticket;

    try
    {
        double finalFee = parkingLot.removeVehicle(reg, &ticket);
        std::cout << "\n========================================\n";
        std::cout << "            CHECKOUT RECEIPT            \n";
        std::cout << "========================================\n";
        std::cout << "Ticket ID       : " << ticket.getTicketId() << "\n";
        std::cout << "Released Space  : Slot #" << ticket.getAssignedSlot() << "\n";
        std::cout << "Registration No : " << ticket.getRegistrationNumber() << "\n";
        std::cout << "Owner Name      : " << ticket.getOwnerName() << "\n";
        std::cout << "Vehicle Type    : " << ParkingFeeCalculator::vehicleTypeToString(ticket.getVehicleType()) << "\n";
        std::cout << "Duration Parked : " << ticket.getParkingHours() << " hrs\n";
        std::cout << "Total Fee Paid  : Rs. " << std::fixed << std::setprecision(2) << finalFee << "\n";
        std::cout << "========================================\n";
        std::cout << "[Success] Slot #" << ticket.getAssignedSlot() << " is now released and available.\n";
    }
    catch (const VehicleNotFoundException& ex)
    {
        std::cout << "\n[Error] " << ex.what() << "\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "\n[Unexpected Error] " << ex.what() << "\n";
    }
}

void handleShowSpaces(const ParkingLot& parkingLot)
{
    std::cout << "\n========================================\n";
    std::cout << "         PARKING SPACES STATUS          \n";
    std::cout << "========================================\n";
    std::cout << "Total Capacity  : " << parkingLot.getCapacity() << " spaces\n";
    std::cout << "Occupied Spaces : " << parkingLot.getOccupiedCount() << "\n";
    std::cout << "Available Spaces: " << parkingLot.getAvailableCount() << "\n\n";

    std::cout << "Available Slots: ";
    auto available = parkingLot.getAvailableSlots();
    if (available.empty())
    {
        std::cout << "None (Full)";
    }
    else
    {
        for (size_t i = 0; i < available.size(); ++i)
        {
            std::cout << "#" << available[i] << (i + 1 < available.size() ? ", " : "");
        }
    }
    std::cout << "\n";

    std::cout << "Occupied Slots : ";
    auto occupied = parkingLot.getOccupiedSlots();
    if (occupied.empty())
    {
        std::cout << "None";
    }
    else
    {
        for (size_t i = 0; i < occupied.size(); ++i)
        {
            std::cout << "#" << occupied[i] << (i + 1 < occupied.size() ? ", " : "");
        }
    }
    std::cout << "\n========================================\n";
}

void handleShowStatistics(const ParkingLot& parkingLot)
{
    // Demonstrate friend function
    showTotalVehicles();

    std::cout << "\n--- REAL-TIME OCCUPANCY METRICS ---\n";
    std::cout << "Currently Parked Vehicles : " << parkingLot.getOccupiedCount() << "\n";
    std::cout << "Available Spaces Remaining: " << parkingLot.getAvailableCount() << "\n";
    double occupancyRate = (static_cast<double>(parkingLot.getOccupiedCount()) / parkingLot.getCapacity()) * 100.0;
    std::cout << "Current Occupancy Rate    : " << std::fixed << std::setprecision(1) << occupancyRate << "%\n";

    double totalActiveBilling = 0.0;
    for (const auto& slot : parkingLot.getSlots())
    {
        if (slot.isOccupied && slot.vehicle)
        {
            totalActiveBilling += slot.vehicle->calculateCharge();
        }
    }
    std::cout << "Current Accrued Fees      : Rs. " << std::fixed << std::setprecision(2) << totalActiveBilling << "\n";
    std::cout << "-----------------------------------\n";
}

void handleSaveData(const ParkingLot& parkingLot)
{
    std::cout << "\nSaving parking records to '" << DATA_FILE_PATH << "'...\n";
    if (FileManager::saveToFile(DATA_FILE_PATH, parkingLot))
    {
        std::cout << "[Success] Records saved successfully to " << DATA_FILE_PATH << "\n";
    }
    else
    {
        std::cout << "[Error] Failed to save parking records!\n";
    }
}

int main()
{
    // Template function usage
    showMessage("==================================================");
    showMessage("   PARKING LOT MANAGEMENT SYSTEM (OOP in C++17)   ");
    showMessage("==================================================");

    ParkingLot parkingLot(10);

    // Auto-load saved records upon application startup
    std::cout << "[Initialization] Loading existing parking records from disk...\n";
    if (FileManager::loadFromFile(DATA_FILE_PATH, parkingLot))
    {
        std::cout << "[Initialization] " << parkingLot.getOccupiedCount()
                  << " vehicle(s) restored from storage.\n";
    }

    int choice = 0;
    do
    {
        std::cout << "\n\n================== MAIN MENU ==================\n";
        std::cout << "1. Park a Vehicle\n";
        std::cout << "2. Display All Parked Vehicles\n";
        std::cout << "3. Search Vehicle\n";
        std::cout << "4. Remove Vehicle and Calculate Fee\n";
        std::cout << "5. Show Available Parking Spaces\n";
        std::cout << "6. Show Parking Statistics\n";
        std::cout << "7. Save Data\n";
        std::cout << "8. Exit\n";
        std::cout << "===============================================\n";

        choice = getValidInteger("Enter your choice (1-8): ", 1, 8);

        switch (choice)
        {
            case 1:
                handleParkVehicle(parkingLot);
                break;
            case 2:
                // Demonstrates overloaded displayVehicles()
                parkingLot.displayVehicles();
                break;
            case 3:
                handleSearchVehicle(parkingLot);
                break;
            case 4:
                handleRemoveVehicle(parkingLot);
                break;
            case 5:
                handleShowSpaces(parkingLot);
                break;
            case 6:
                handleShowStatistics(parkingLot);
                break;
            case 7:
                handleSaveData(parkingLot);
                break;
            case 8:
                std::cout << "\n[Shutdown] Performing automatic data save...\n";
                FileManager::saveToFile(DATA_FILE_PATH, parkingLot);
                showMessage("Data safely stored. Thank you for using Parking Lot Management System!");
                break;
            default:
                std::cout << "[Error] Invalid option selected.\n";
                break;
        }

    } while (choice != 8);

    return 0;
}
