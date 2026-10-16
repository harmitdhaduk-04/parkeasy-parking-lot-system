#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include "ParkingLot.h"

/**
 * @brief Class responsible for persistence and serialization of parking records.
 *
 * Separates file I/O operations from core business logic (Single Responsibility Principle).
 * Implements atomic saving (write to temp file, then rename) to prevent data corruption.
 */
class FileManager
{
private:
    static const char DELIMITER = '|';

    // Helper to ensure target directory exists
    static bool ensureDirectoryExists(const std::string& filepath);

public:
    /**
     * @brief Saves active parking records to disk atomically.
     * @param filepath Path to save destination (e.g. "data/parking.txt")
     * @param parkingLot Reference to ParkingLot instance
     * @return true if successful, false otherwise
     */
    static bool saveToFile(const std::string& filepath, const ParkingLot& parkingLot);

    /**
     * @brief Loads parked vehicle records from disk into the parking lot.
     * @param filepath Path to source file
     * @param parkingLot Target ParkingLot instance
     * @return true if successful or file does not exist (clean start), false on parse failure
     */
    static bool loadFromFile(const std::string& filepath, ParkingLot& parkingLot);
};

#endif // FILE_MANAGER_H
