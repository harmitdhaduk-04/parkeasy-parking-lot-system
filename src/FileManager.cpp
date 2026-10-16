#include "../include/FileManager.h"
#include "../include/Car.h"
#include "../include/Bike.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

bool FileManager::ensureDirectoryExists(const std::string& filepath)
{
    size_t lastSlash = filepath.find_last_of("/\\");
    if (lastSlash == std::string::npos)
    {
        return true; // No directory part
    }

    std::string dirPath = filepath.substr(0, lastSlash);
    if (dirPath.empty())
    {
        return true;
    }

#ifdef _WIN32
    _mkdir(dirPath.c_str());
#else
    mkdir(dirPath.c_str(), 0755);
#endif
    return true;
}

bool FileManager::saveToFile(const std::string& filepath, const ParkingLot& parkingLot)
{
    ensureDirectoryExists(filepath);

    std::string tempFilepath = filepath + ".tmp";
    std::ofstream outFile(tempFilepath);
    if (!outFile.is_open())
    {
        std::cerr << "[FileManager Error] Could not open temporary file for writing: "
                  << tempFilepath << std::endl;
        return false;
    }

    // Write file header comment
    outFile << "# Parking Lot Management System Records\n";
    outFile << "# Format: TYPE|REGISTRATION|OWNER|HOURS|SLOT|TICKET_ID|ENTRY_TIME\n";

    const auto& slots = parkingLot.getSlots();
    for (const auto& slot : slots)
    {
        if (slot.isOccupied && slot.vehicle)
        {
            std::string typeStr = slot.vehicle->getTypeString();
            std::string reg = slot.vehicle->getRegistrationNumber();
            std::string owner = slot.vehicle->getOwnerName();
            int hours = slot.vehicle->getParkingHours();
            int slotNum = slot.slotNumber;
            std::string ticketId = slot.ticket ? slot.ticket->getTicketId() : "";
            std::string entryTime = slot.ticket ? slot.ticket->getEntryTime() : "";

            outFile << typeStr << DELIMITER
                    << reg << DELIMITER
                    << owner << DELIMITER
                    << hours << DELIMITER
                    << slotNum << DELIMITER
                    << ticketId << DELIMITER
                    << entryTime << "\n";
        }
    }

    outFile.close();
    if (outFile.fail())
    {
        std::cerr << "[FileManager Error] Failed to flush data to disk." << std::endl;
        std::remove(tempFilepath.c_str());
        return false;
    }

    // Atomic replacement: remove old file if exists, then rename temporary file
    std::remove(filepath.c_str());
    if (std::rename(tempFilepath.c_str(), filepath.c_str()) != 0)
    {
        std::cerr << "[FileManager Error] Failed to rename temp file to target destination." << std::endl;
        std::remove(tempFilepath.c_str());
        return false;
    }

    return true;
}

bool FileManager::loadFromFile(const std::string& filepath, ParkingLot& parkingLot)
{
    std::ifstream inFile(filepath);
    if (!inFile.is_open())
    {
        // Missing file on startup is a normal condition for fresh installation
        return true;
    }

    parkingLot.clear();

    std::string line;
    int lineNum = 0;
    while (std::getline(inFile, line))
    {
        lineNum++;
        if (line.empty() || line[0] == '#')
        {
            continue; // Skip comments and empty lines
        }

        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, DELIMITER))
        {
            tokens.push_back(token);
        }

        if (tokens.size() < 5)
        {
            std::cerr << "[FileManager Warning] Malformed record at line " << lineNum
                      << " (insufficient tokens). Skipping.\n";
            continue;
        }

        std::string typeStr = tokens[0];
        std::string reg = tokens[1];
        std::string owner = tokens[2];
        int hours = 0;
        int slot = 0;

        try
        {
            hours = std::stoi(tokens[3]);
            slot = std::stoi(tokens[4]);
        }
        catch (const std::exception& e)
        {
            std::cerr << "[FileManager Warning] Number parsing error at line " << lineNum
                      << ": " << e.what() << ". Skipping.\n";
            continue;
        }

        std::string ticketId = tokens.size() > 5 ? tokens[5] : "";
        std::string entryTime = tokens.size() > 6 ? tokens[6] : "";

        VehicleType vType = ParkingFeeCalculator::stringToVehicleType(typeStr);
        std::unique_ptr<Vehicle> vehicle;

        if (vType == VehicleType::Car)
        {
            vehicle = std::make_unique<Car>(reg, owner, hours);
        }
        else if (vType == VehicleType::Bike)
        {
            vehicle = std::make_unique<Bike>(reg, owner, hours);
        }
        else
        {
            std::cerr << "[FileManager Warning] Unknown vehicle type '" << typeStr
                      << "' at line " << lineNum << ". Skipping.\n";
            continue;
        }

        ParkingTicket ticket(reg, owner, vType, slot, hours, entryTime, ticketId);

        try
        {
            parkingLot.restoreVehicle(std::move(vehicle), slot, ticket);
        }
        catch (const std::exception& ex)
        {
            std::cerr << "[FileManager Warning] Failed to restore vehicle from line "
                      << lineNum << ": " << ex.what() << ". Skipping.\n";
        }
    }

    inFile.close();
    return true;
}
