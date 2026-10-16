#include "../include/Vehicle.h"
#include <iomanip>

// Initialize static member tracking total vehicle objects created during program execution
int Vehicle::totalVehicles = 0;

Vehicle::Vehicle(const std::string& reg, const std::string& owner, int hours)
    : registrationNumber(reg), ownerName(owner), parkingHours(hours)
{
    // Increment lifetime vehicle instance counter
    totalVehicles++;
}

Vehicle::~Vehicle()
{
    // Virtual destructor body
}

std::string Vehicle::getRegistrationNumber() const
{
    return registrationNumber;
}

std::string Vehicle::getOwnerName() const
{
    return ownerName;
}

int Vehicle::getParkingHours() const
{
    return parkingHours;
}

void Vehicle::setParkingHours(int hours)
{
    parkingHours = hours;
}

void Vehicle::display(std::ostream& os) const
{
    os << "Vehicle Number : " << registrationNumber << "\n";
    os << "Owner Name     : " << ownerName << "\n";
    os << "Parking Hours  : " << parkingHours << " hrs\n";
}

// Function overloading: Search by registration number parameter
void Vehicle::searchVehicle(const std::string& queryReg) const
{
    if (registrationNumber == queryReg)
    {
        std::cout << "[Match Found] Vehicle Number: " << registrationNumber
                  << " owned by " << ownerName << std::endl;
    }
    else
    {
        std::cout << "[No Match] Registration does not match." << std::endl;
    }
}

// Function overloading: Search/Display vehicle's own info without parameters
void Vehicle::searchVehicle() const
{
    std::cout << "Vehicle Registration: " << registrationNumber << std::endl;
}

bool Vehicle::matchesRegistration(const std::string& reg) const
{
    return registrationNumber == reg;
}

// Operator overloading: Compare two vehicles by registration number
bool Vehicle::operator==(const Vehicle& other) const
{
    return this->registrationNumber == other.registrationNumber;
}

bool Vehicle::operator!=(const Vehicle& other) const
{
    return !(*this == other);
}

// ================= FRIEND FUNCTIONS =================
// Friend function accessing static private/protected member
void showTotalVehicles()
{
    std::cout << "\n========================================\n"
              << " Total Vehicle Objects Created: " << Vehicle::totalVehicles << "\n"
              << "========================================\n";
}

// Friend function accessing protected vehicle attributes directly
void showVehicleDetails(const Vehicle& v)
{
    std::cout << "[Friend Function Inspection] Registration: " << v.registrationNumber
              << ", Owner: " << v.ownerName
              << ", Hours: " << v.parkingHours << "\n";
}
