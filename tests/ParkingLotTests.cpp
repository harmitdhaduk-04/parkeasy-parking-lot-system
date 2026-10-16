#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include <cstdio>
#include "../include/ParkingLot.h"
#include "../include/Car.h"
#include "../include/Bike.h"
#include "../include/ParkingFeeCalculator.h"
#include "../include/FileManager.h"

// ================= TEST SUITE INFRASTRUCTURE =================

int totalTests = 0;
int passedTests = 0;
int failedTests = 0;

#define TEST_CASE(name) \
    void name(); \
    struct Register_##name { \
        Register_##name() { \
            testFunctions.push_back({#name, name}); \
        } \
    } instance_##name; \
    void name()

struct TestCaseEntry {
    std::string name;
    void (*func)();
};

std::vector<TestCaseEntry> testFunctions;

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "  FAILED: " #condition " at line " << __LINE__ << std::endl; \
            throw std::runtime_error("Assertion failed: " #condition); \
        } \
    } while (0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            std::cerr << "  FAILED: NOT(" #condition ") at line " << __LINE__ << std::endl; \
            throw std::runtime_error("Assertion failed: NOT(" #condition ")"); \
        } \
    } while (0)

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "  FAILED: " #a " == " #b " (" << (a) << " != " << (b) << ") at line " << __LINE__ << std::endl; \
            throw std::runtime_error("Assertion failed: equality"); \
        } \
    } while (0)

#define ASSERT_NEAR(a, b, eps) \
    do { \
        if (std::fabs((a) - (b)) > (eps)) { \
            std::cerr << "  FAILED: |" #a " - " #b "| <= " #eps " (" << (a) << " vs " << (b) << ") at line " << __LINE__ << std::endl; \
            throw std::runtime_error("Assertion failed: near equality"); \
        } \
    } while (0)

#define ASSERT_THROW(statement, ExceptionType) \
    do { \
        bool threwExpected = false; \
        try { \
            statement; \
        } catch (const ExceptionType&) { \
            threwExpected = true; \
        } catch (...) {} \
        if (!threwExpected) { \
            std::cerr << "  FAILED: Expected " #ExceptionType " for " #statement " at line " << __LINE__ << std::endl; \
            throw std::runtime_error("Expected exception not thrown: " #ExceptionType); \
        } \
    } while (0)

#define ASSERT_NO_THROW(statement) \
    do { \
        try { \
            statement; \
        } catch (const std::exception& e) { \
            std::cerr << "  FAILED: Unexpected exception for " #statement ": " << e.what() << " at line " << __LINE__ << std::endl; \
            throw std::runtime_error("Unexpected exception thrown"); \
        } catch (...) { \
            std::cerr << "  FAILED: Unknown exception for " #statement " at line " << __LINE__ << std::endl; \
            throw std::runtime_error("Unknown exception thrown"); \
        } \
    } while (0)

const std::string TEST_DATA_FILE = "data/test_parking_persistence.txt";

void cleanupTestFile()
{
    std::remove(TEST_DATA_FILE.c_str());
    std::remove((TEST_DATA_FILE + ".tmp").c_str());
}

// ================= TEST SCENARIOS =================

// Scenario 1: Successfully park a car
TEST_CASE(Test_01_SuccessfullyParkCar)
{
    ParkingLot lot(10);
    auto car = std::make_unique<Car>("KA-01-AB-1234", "Alice", 3);
    ParkingTicket ticket = lot.parkVehicle(std::move(car));

    ASSERT_EQ(ticket.getRegistrationNumber(), "KA-01-AB-1234");
    ASSERT_EQ(ticket.getOwnerName(), "Alice");
    ASSERT_EQ(ticket.getAssignedSlot(), 1);
    ASSERT_EQ(ticket.getParkingHours(), 3);
    ASSERT_EQ(lot.getOccupiedCount(), 1);
    ASSERT_EQ(lot.getAvailableCount(), 9);
}

// Scenario 2: Successfully park a bike
TEST_CASE(Test_02_SuccessfullyParkBike)
{
    ParkingLot lot(10);
    auto bike = std::make_unique<Bike>("MH-02-XY-9999", "Bob", 4);
    ParkingTicket ticket = lot.parkVehicle(std::move(bike));

    ASSERT_EQ(ticket.getRegistrationNumber(), "MH-02-XY-9999");
    ASSERT_EQ(ticket.getOwnerName(), "Bob");
    ASSERT_EQ(ticket.getAssignedSlot(), 1);
    ASSERT_EQ(ticket.getParkingHours(), 4);
    ASSERT_EQ(lot.getOccupiedCount(), 1);
}

// Scenario 3: Correct parking fee for a car (Rs 50/hour)
TEST_CASE(Test_03_CorrectParkingFeeCar)
{
    ParkingLot lot(10);
    auto car = std::make_unique<Car>("DL-03-C-1111", "Charlie", 5);
    lot.parkVehicle(std::move(car));

    ParkingTicket ticket;
    double fee = lot.removeVehicle("DL-03-C-1111", &ticket);
    ASSERT_NEAR(fee, 250.0, 0.001); // 5 * 50 = 250
}

// Scenario 4: Correct parking fee for a bike (Rs 20/hour)
TEST_CASE(Test_04_CorrectParkingFeeBike)
{
    ParkingLot lot(10);
    auto bike = std::make_unique<Bike>("TN-07-B-2222", "David", 3);
    lot.parkVehicle(std::move(bike));

    ParkingTicket ticket;
    double fee = lot.removeVehicle("TN-07-B-2222", &ticket);
    ASSERT_NEAR(fee, 60.0, 0.001); // 3 * 20 = 60
}

// Scenario 5: Reject duplicate vehicle registrations
TEST_CASE(Test_05_RejectDuplicateVehicleRegistrations)
{
    ParkingLot lot(10);
    lot.parkVehicle(std::make_unique<Car>("GJ-01-DUP-1", "Emma", 2));

    ASSERT_THROW(
        lot.parkVehicle(std::make_unique<Car>("GJ-01-DUP-1", "Frank", 4)),
        DuplicateVehicleException
    );

    ASSERT_THROW(
        lot.parkVehicle(std::make_unique<Bike>("GJ-01-DUP-1", "Grace", 1)),
        DuplicateVehicleException
    );

    ASSERT_EQ(lot.getOccupiedCount(), 1);
}

// Scenario 6: Reject invalid parking durations (hours <= 0)
TEST_CASE(Test_06_RejectInvalidParkingDurations)
{
    ParkingLot lot(10);
    ASSERT_THROW(
        lot.parkVehicle(std::make_unique<Car>("KA-05-NEG-1", "Hank", 0)),
        InvalidDurationException
    );
    ASSERT_THROW(
        lot.parkVehicle(std::make_unique<Bike>("KA-05-NEG-2", "Ivy", -3)),
        InvalidDurationException
    );
    ASSERT_EQ(lot.getOccupiedCount(), 0);
}

// Scenario 7: Reject parking when capacity is full
TEST_CASE(Test_07_RejectParkingWhenCapacityIsFull)
{
    ParkingLot lot(3); // Small capacity for deterministic full test
    lot.parkVehicle(std::make_unique<Car>("C-1", "Owner1", 2));
    lot.parkVehicle(std::make_unique<Car>("C-2", "Owner2", 2));
    lot.parkVehicle(std::make_unique<Bike>("B-3", "Owner3", 2));

    ASSERT_EQ(lot.getOccupiedCount(), 3);
    ASSERT_EQ(lot.getAvailableCount(), 0);

    ASSERT_THROW(
        lot.parkVehicle(std::make_unique<Car>("C-4", "Owner4", 2)),
        ParkingLotFullException
    );
    ASSERT_EQ(lot.getOccupiedCount(), 3);
}

// Scenario 8: Release a parking space after removal
TEST_CASE(Test_08_ReleaseParkingSpaceAfterRemoval)
{
    ParkingLot lot(5);
    lot.parkVehicle(std::make_unique<Car>("REG-101", "Jack", 3));
    ASSERT_EQ(lot.getOccupiedCount(), 1);
    ASSERT_EQ(lot.getAvailableCount(), 4);

    lot.removeVehicle("REG-101");
    ASSERT_EQ(lot.getOccupiedCount(), 0);
    ASSERT_EQ(lot.getAvailableCount(), 5);
    ASSERT_TRUE(lot.searchVehicle("REG-101") == nullptr);
}

// Scenario 9: Reuse a released parking space
TEST_CASE(Test_09_ReuseReleasedParkingSpace)
{
    ParkingLot lot(3);
    lot.parkVehicle(std::make_unique<Car>("SLOT-1-VEH", "User1", 2)); // Assigns slot 1
    lot.parkVehicle(std::make_unique<Car>("SLOT-2-VEH", "User2", 2)); // Assigns slot 2

    // Remove vehicle in slot 1
    lot.removeVehicle("SLOT-1-VEH");

    // Park another vehicle: should reuse slot 1 (lowest available slot number)
    auto ticket = lot.parkVehicle(std::make_unique<Bike>("SLOT-REUSE", "User3", 1));
    ASSERT_EQ(ticket.getAssignedSlot(), 1);
}

// Scenario 10: Search for an existing vehicle
TEST_CASE(Test_10_SearchForExistingVehicle)
{
    ParkingLot lot(5);
    lot.parkVehicle(std::make_unique<Car>("SEARCH-ME", "Nancy", 4));

    // Overloaded search by string registration
    const Vehicle* vByReg = lot.searchVehicle("SEARCH-ME");
    ASSERT_TRUE(vByReg != nullptr);
    ASSERT_EQ(vByReg->getRegistrationNumber(), "SEARCH-ME");
    ASSERT_EQ(vByReg->getOwnerName(), "Nancy");

    // Overloaded search by slot number (1)
    const Vehicle* vBySlot = lot.searchVehicle(1);
    ASSERT_TRUE(vBySlot != nullptr);
    ASSERT_EQ(vBySlot->getRegistrationNumber(), "SEARCH-ME");
}

// Scenario 11: Handle a nonexistent vehicle search
TEST_CASE(Test_11_HandleNonexistentVehicleSearch)
{
    ParkingLot lot(5);
    lot.parkVehicle(std::make_unique<Car>("EXISTS-1", "Oliver", 2));

    const Vehicle* v1 = lot.searchVehicle("DOES-NOT-EXIST");
    ASSERT_TRUE(v1 == nullptr);

    // Slot 2 is currently empty
    const Vehicle* v2 = lot.searchVehicle(2);
    ASSERT_TRUE(v2 == nullptr);

    // Invalid slot index outside capacity
    const Vehicle* v3 = lot.searchVehicle(99);
    ASSERT_TRUE(v3 == nullptr);
}

// Scenario 12: Handle removal of a nonexistent vehicle
TEST_CASE(Test_12_HandleRemovalOfNonexistentVehicle)
{
    ParkingLot lot(5);
    lot.parkVehicle(std::make_unique<Car>("REAL-1", "Paul", 2));

    ASSERT_THROW(
        lot.removeVehicle("GHOST-REG"),
        VehicleNotFoundException
    );

    // Original vehicle must remain untouched
    ASSERT_EQ(lot.getOccupiedCount(), 1);
    ASSERT_TRUE(lot.searchVehicle("REAL-1") != nullptr);
}

// Scenario 13: Save and reload parked vehicles correctly
TEST_CASE(Test_13_SaveAndReloadParkedVehiclesCorrectly)
{
    cleanupTestFile();

    ParkingLot saveLot(5);
    saveLot.parkVehicle(std::make_unique<Car>("SAVE-CAR-1", "Quinn", 3));
    saveLot.parkVehicle(std::make_unique<Bike>("SAVE-BIKE-2", "Rachel", 4));

    bool saveOk = FileManager::saveToFile(TEST_DATA_FILE, saveLot);
    ASSERT_TRUE(saveOk);

    ParkingLot loadLot(5);
    bool loadOk = FileManager::loadFromFile(TEST_DATA_FILE, loadLot);
    ASSERT_TRUE(loadOk);

    ASSERT_EQ(loadLot.getOccupiedCount(), 2);
    ASSERT_TRUE(loadLot.searchVehicle("SAVE-CAR-1") != nullptr);
    ASSERT_TRUE(loadLot.searchVehicle("SAVE-BIKE-2") != nullptr);

    cleanupTestFile();
}

// Scenario 14: Preserve vehicle type and parking space after reload
TEST_CASE(Test_14_PreserveVehicleTypeAndParkingSpaceAfterReload)
{
    cleanupTestFile();

    ParkingLot lotA(5);
    lotA.parkVehicle(std::make_unique<Car>("TYPE-CAR", "Steve", 2));   // Slot 1
    lotA.parkVehicle(std::make_unique<Bike>("TYPE-BIKE", "Tina", 5));   // Slot 2

    FileManager::saveToFile(TEST_DATA_FILE, lotA);

    ParkingLot lotB(5);
    FileManager::loadFromFile(TEST_DATA_FILE, lotB);

    const Vehicle* carPtr = lotB.searchVehicle("TYPE-CAR");
    const Vehicle* bikePtr = lotB.searchVehicle("TYPE-BIKE");

    ASSERT_TRUE(carPtr != nullptr);
    ASSERT_TRUE(bikePtr != nullptr);

    ASSERT_EQ(carPtr->getType(), VehicleType::Car);
    ASSERT_EQ(carPtr->getTypeString(), "Car");
    ASSERT_NEAR(carPtr->calculateCharge(), 100.0, 0.001); // 2 * 50

    ASSERT_EQ(bikePtr->getType(), VehicleType::Bike);
    ASSERT_EQ(bikePtr->getTypeString(), "Bike");
    ASSERT_NEAR(bikePtr->calculateCharge(), 100.0, 0.001); // 5 * 20

    // Verify slots preserved exactly
    const Vehicle* slot1Veh = lotB.searchVehicle(1);
    const Vehicle* slot2Veh = lotB.searchVehicle(2);
    ASSERT_TRUE(slot1Veh != nullptr);
    ASSERT_TRUE(slot2Veh != nullptr);
    ASSERT_EQ(slot1Veh->getRegistrationNumber(), "TYPE-CAR");
    ASSERT_EQ(slot2Veh->getRegistrationNumber(), "TYPE-BIKE");

    cleanupTestFile();
}

// Scenario 15: Verify vehicle comparison using overloaded == operator
TEST_CASE(Test_15_VerifyVehicleComparisonUsingOverloadedOperator)
{
    Car c1("REG-SAME", "UserA", 2);
    Car c2("REG-SAME", "UserB", 4);
    Bike b1("REG-DIFF", "UserC", 1);

    // Operator == compares registration numbers
    ASSERT_TRUE(c1 == c2);
    ASSERT_FALSE(c1 == b1);
    ASSERT_TRUE(c1 != b1);
}

// Scenario 16: Verify that vehicle counts behave as documented
TEST_CASE(Test_16_VerifyVehicleCountsBehaveAsDocumented)
{
    int initialLifetimeObjects = Vehicle::totalVehicles;

    ParkingLot lot(5);
    ASSERT_EQ(lot.getOccupiedCount(), 0);

    lot.parkVehicle(std::make_unique<Car>("COUNT-1", "Uma", 1));
    lot.parkVehicle(std::make_unique<Bike>("COUNT-2", "Victor", 2));

    // Currently parked count is strictly 2
    ASSERT_EQ(lot.getOccupiedCount(), 2);
    // Lifetime objects created increased by at least 2
    ASSERT_TRUE(Vehicle::totalVehicles >= initialLifetimeObjects + 2);

    // When 1 vehicle is removed, occupied count decreases
    lot.removeVehicle("COUNT-1");
    ASSERT_EQ(lot.getOccupiedCount(), 1);

    // But lifetime total vehicle objects instantiated is not decremented
    ASSERT_TRUE(Vehicle::totalVehicles >= initialLifetimeObjects + 2);
}

// Scenario 17: Verify that failed parking operations do not corrupt parking records
TEST_CASE(Test_17_VerifyFailedParkingOperationsDoNotCorruptRecords)
{
    ParkingLot lot(2);
    lot.parkVehicle(std::make_unique<Car>("VALID-1", "Wendy", 2));

    // Attempt 1: Park duplicate vehicle
    try {
        lot.parkVehicle(std::make_unique<Car>("VALID-1", "DuplicateWendy", 3));
    } catch (...) {}

    // Attempt 2: Park vehicle with invalid hours
    try {
        lot.parkVehicle(std::make_unique<Bike>("INVALID-HOURS", "Xavier", 0));
    } catch (...) {}

    // Attempt 3: Park null pointer
    try {
        lot.parkVehicle(nullptr);
    } catch (...) {}

    // Assert that the parking lot state is completely uncorrupted
    ASSERT_EQ(lot.getOccupiedCount(), 1);
    ASSERT_EQ(lot.getAvailableCount(), 1);

    const Vehicle* v = lot.searchVehicle("VALID-1");
    ASSERT_TRUE(v != nullptr);
    ASSERT_EQ(v->getOwnerName(), "Wendy");
    ASSERT_EQ(v->getParkingHours(), 2);

    // Now park a second valid car, should successfully take slot 2
    auto ticket = lot.parkVehicle(std::make_unique<Bike>("VALID-2", "Yara", 3));
    ASSERT_EQ(ticket.getAssignedSlot(), 2);
    ASSERT_EQ(lot.getOccupiedCount(), 2);
    ASSERT_EQ(lot.getAvailableCount(), 0);
}

// ================= TEST RUNNER MAIN =================

int main()
{
    std::cout << "========================================================\n";
    std::cout << "     RUNNING PARKING LOT MANAGEMENT SYSTEM UNIT TESTS   \n";
    std::cout << "========================================================\n\n";

    cleanupTestFile();

    totalTests = static_cast<int>(testFunctions.size());

    for (const auto& test : testFunctions)
    {
        std::cout << "[ RUN      ] " << test.name << "..." << std::flush;
        try
        {
            test.func();
            passedTests++;
            std::cout << " [ PASS ]\n";
        }
        catch (const std::exception& ex)
        {
            failedTests++;
            std::cout << " [ FAIL ] (" << ex.what() << ")\n";
        }
        catch (...)
        {
            failedTests++;
            std::cout << " [ FAIL ] (Unknown exception)\n";
        }
    }

    cleanupTestFile();

    std::cout << "\n========================================================\n";
    std::cout << " TEST EXECUTION SUMMARY\n";
    std::cout << " Total Scenarios Executed : " << totalTests << "\n";
    std::cout << " Passed                   : " << passedTests << "\n";
    std::cout << " Failed                   : " << failedTests << "\n";
    std::cout << "========================================================\n";

    if (failedTests == 0)
    {
        std::cout << "ALL TESTS PASSED SUCCESSFULLY! (100% Pass Rate)\n";
        return 0;
    }
    else
    {
        std::cerr << failedTests << " TEST(S) FAILED!\n";
        return 1;
    }
}
