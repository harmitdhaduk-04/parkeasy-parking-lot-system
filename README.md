# Parking Lot Management System (OOP in C++17)

A modular, robust, console-based Parking Lot Management System implemented in **C++17** using Object-Oriented Programming (OOP) principles, clean architecture, RAII memory management, and CMake build automation.

---

## 1. Project Overview

The **Parking Lot Management System** models a real-world parking facility. It assigns parking spaces, issues parking tickets, calculates fees polymorphically based on vehicle type and duration, releases and reuses vacated spaces, and persists all active parking records to disk atomically.

The project is structured into separated header and implementation files, adhering to the Single Responsibility Principle (SRP) and C++ core guidelines.

---

## 2. Key Features

1. **Park a Vehicle**:
   - Supports Cars (₹50/hr) and Bikes (₹20/hr).
   - Validates input, rejects negative/zero durations, and rejects duplicate registration numbers.
   - Enforces parking capacity limits (default: 10 spaces).
   - Assigns the lowest available parking slot and generates a unique ticket (`TKT-1001`, `TKT-1002`, etc.) with entry timestamps.

2. **Display All Parked Vehicles**:
   - Uses runtime polymorphism to display vehicle-specific details and rate estimates.
   - Shows assigned space number, ticket ID, registration number, owner name, parking hours, and vehicle type.

3. **Search for a Vehicle (Function Overloading)**:
   - Search by registration number (`std::string`).
   - Search by parking space/slot number (`int`).

4. **Remove Vehicle & Billing**:
   - Removes vehicle by registration number.
   - Calculates and displays the final fee on an itemized checkout receipt.
   - Releases the slot immediately for subsequent reuse.
   - Gracefully handles non-existent vehicle lookups without crashing.

5. **Slot Tracking & Space Reuse**:
   - Explicitly tracks occupied and available slots.
   - Always reuses vacated slots using lowest-index slot allocation.

6. **Real-time Occupancy Metrics & Statistics**:
   - Differentiates between total vehicle objects instantiated in memory (via static member & friend function) vs. currently parked vehicles.
   - Shows current occupancy percentage and accrued revenue.

7. **Atomic File Persistence**:
   - Loads parking records automatically on startup from `data/parking.txt`.
   - Saves records upon explicit menu request and automatically during shutdown.
   - Implements atomic writes (`.tmp` write followed by rename) to prevent file truncation or corruption.

8. **Automated Testing**:
   - Unit test suite validating 17 critical test scenarios with 100% pass rate.
   - Integrated with CMake CTest.

---

## 3. OOP Concepts Demonstrated

| OOP Concept | Implementation & Demonstration | Relevant Files |
| :--- | :--- | :--- |
| **Encapsulation** | Private/protected attributes (`registrationNumber`, `ownerName`, `parkingHours`, `slots`, `capacity`); validation via public interfaces; prevents invalid states. | [`Vehicle.h`](include/Vehicle.h), [`ParkingLot.h`](include/ParkingLot.h), [`ParkingTicket.h`](include/ParkingTicket.h) |
| **Abstraction** | Abstract base class `Vehicle` with pure virtual functions `calculateCharge()`, `getType()`, `getTypeString()`. Hides rate calculations and internal slot tracking. | [`Vehicle.h`](include/Vehicle.h), [`ParkingFeeCalculator.h`](include/ParkingFeeCalculator.h) |
| **Inheritance** | `Car` and `Bike` inherit publicly from `Vehicle`, reusing base attributes while providing specialized implementations. | [`Car.h`](include/Car.h), [`Bike.h`](include/Bike.h) |
| **Polymorphism** | Runtime polymorphic dispatch using `std::unique_ptr<Vehicle>`. Calling `calculateCharge()` or `display()` through base class pointers invokes derived class overrides. | [`ParkingLot.cpp`](src/ParkingLot.cpp), [`Car.cpp`](src/Car.cpp), [`Bike.cpp`](src/Bike.cpp) |
| **Function Overloading** | Overloaded `searchVehicle(string)` vs `searchVehicle(int)`; overloaded `displayVehicles()` vs `displayVehicles(ostream&)`. | [`Vehicle.h`](include/Vehicle.h), [`ParkingLot.h`](include/ParkingLot.h) |
| **Operator Overloading** | Overloaded `operator==` and `operator!=` to compare vehicles by registration number; `operator<<` for `VehicleType`. | [`Vehicle.cpp`](src/Vehicle.cpp), [`ParkingFeeCalculator.h`](include/ParkingFeeCalculator.h) |
| **Static Members** | `Vehicle::totalVehicles` tracks total vehicle objects created in memory during program lifecycle. Distinct from active slot count `ParkingLot::getOccupiedCount()`. | [`Vehicle.h`](include/Vehicle.h), [`Vehicle.cpp`](src/Vehicle.cpp) |
| **Friend Function** | `showTotalVehicles()` and `showVehicleDetails(const Vehicle&)` access protected/private members directly. | [`Vehicle.h`](include/Vehicle.h), [`Vehicle.cpp`](src/Vehicle.cpp) |
| **Templates** | Generic template function `showMessage<T>(T message)` for generic console printing. | [`Vehicle.h`](include/Vehicle.h), [`main.cpp`](src/main.cpp) |
| **Exception Handling** | Custom exception hierarchy derived from `std::runtime_error`: `ParkingLotFullException`, `DuplicateVehicleException`, `VehicleNotFoundException`, `InvalidDurationException`. | [`ParkingLot.h`](include/ParkingLot.h) |
| **RAII & Smart Pointers** | Uses `std::unique_ptr` and standard containers (`std::vector`, `std::string`) for leak-free, deterministic resource management without manual raw `new`/`delete`. | [`ParkingLot.h`](include/ParkingLot.h), [`ParkingLot.cpp`](src/ParkingLot.cpp) |

---

## 4. Project Directory Structure

```text
ParkingLotManagementSystem/
│
├── CMakeLists.txt              # CMake build configuration for Library, App, and Tests
├── README.md                   # Complete academic and architectural documentation
│
├── include/                    # Header files (Declarations & Interfaces)
│   ├── Vehicle.h               # Abstract base class for all vehicles
│   ├── Car.h                   # Derived Car class
│   ├── Bike.h                  # Derived Bike class
│   ├── ParkingLot.h            # Lot management, slot tracking, exceptions, business rules
│   ├── ParkingTicket.h         # Ticket model, slot assignment, billing info
│   ├── ParkingFeeCalculator.h  # Centralized fee and rate calculator
│   └── FileManager.h           # Atomic file persistence manager
│
├── src/                        # Source files (Implementations)
│   ├── Vehicle.cpp
│   ├── Car.cpp
│   ├── Bike.cpp
│   ├── ParkingLot.cpp
│   ├── ParkingTicket.cpp
│   ├── ParkingFeeCalculator.cpp
│   ├── FileManager.cpp
│   └── main.cpp                # Console UI, menu controller, user interaction
│
├── tests/
│   └── ParkingLotTests.cpp     # Automated test suite (17 distinct test scenarios)
│
└── data/
    └── parking.txt             # Persistent storage file for active parking records
```

---

## 5. Class Responsibilities

- **`Vehicle`**: Abstract base class defining common vehicle properties (`registrationNumber`, `ownerName`, `parkingHours`), operator overloads (`==`), overloaded search methods, and pure virtual fee calculation.
- **`Car`**: Derived class implementing car-specific fee calculation (₹50/hour) and formatted display.
- **`Bike`**: Derived class implementing bike-specific fee calculation (₹20/hour) and formatted display.
- **`ParkingTicket`**: Encapsulates ticket identification (`TKT-xxxx`), assigned slot, entry timestamp, and fee estimates.
- **`ParkingFeeCalculator`**: Encapsulates rate rules (₹50 for Cars, ₹20 for Bikes) in one centralized place, preventing hardcoded magic numbers.
- **`ParkingLot`**: Manages capacity, slot state, vehicle parking, vehicle removal, space allocation and reuse, and enforces business rules.
- **`FileManager`**: Handles file serialization, parsing, and atomic writes (`temp file -> rename`) to `data/parking.txt`.
- **`main.cpp`**: Console menu presentation, robust input validation, and user workflow dispatch.

---

## 6. Fee Calculation Rules

| Vehicle Type | Rate per Hour | Example (3 Hours) |
| :--- | :--- | :--- |
| **Car** | ₹50.00 / hr | 3 × ₹50 = ₹150.00 |
| **Bike** | ₹20.00 / hr | 3 × ₹20 = ₹60.00 |

*Rates are managed solely inside `ParkingFeeCalculator`.*

---

## 7. File Persistence Format

Records are saved in `data/parking.txt` using a pipe-delimited format:

```text
# Parking Lot Management System Records
# Format: TYPE|REGISTRATION|OWNER|HOURS|SLOT|TICKET_ID|ENTRY_TIME
Car|KA-01-AB-1234|Alice|3|1|TKT-1001|2026-10-16 17:56:47
Bike|MH-02-XY-9999|Bob|4|2|TKT-1002|2026-10-16 17:58:12
```

### Persistence Features:
- Preserves vehicle type (`Car` / `Bike`), slot number, ticket ID, and entry timestamp.
- Atomic write: writes to `parking.txt.tmp` first, then replaces `parking.txt` to eliminate corruption risk.
- Handles missing or empty files gracefully on startup.

---

## 8. Build, Run, and Test Instructions

### Prerequisites
- C++17 compatible compiler (e.g. GCC / MinGW, Clang, or MSVC)
- CMake 3.10 or higher
- Make / MinGW32-Make / Ninja

### Building the Project
From the project root directory:

```bash
# 1. Configure build files
cmake -S . -B build -G "MinGW Makefiles"

# 2. Compile application and tests
cmake --build build
```

### Running the Application
```bash
./build/ParkingLotApp.exe
```

### Running the Automated Tests
Run via CMake CTest:
```bash
ctest --test-dir build --output-on-failure
```

Or run the test executable directly:
```bash
./build/ParkingLotTests.exe
```

---

## 9. Automated Unit Test Scenarios (17 Scenarios)

The test suite in `tests/ParkingLotTests.cpp` executes the following 17 scenarios:

1. `Test_01_SuccessfullyParkCar`: Verifies parking a car, slot allocation, and ticket creation.
2. `Test_02_SuccessfullyParkBike`: Verifies parking a bike and ticket creation.
3. `Test_03_CorrectParkingFeeCar`: Validates ₹50/hr rate calculation.
4. `Test_04_CorrectParkingFeeBike`: Validates ₹20/hr rate calculation.
5. `Test_05_RejectDuplicateVehicleRegistrations`: Asserts `DuplicateVehicleException` on duplicate registration.
6. `Test_06_RejectInvalidParkingDurations`: Asserts `InvalidDurationException` when hours ≤ 0.
7. `Test_07_RejectParkingWhenCapacityIsFull`: Asserts `ParkingLotFullException` when capacity is exceeded.
8. `Test_08_ReleaseParkingSpaceAfterRemoval`: Asserts slot release and occupancy count decrements.
9. `Test_09_ReuseReleasedParkingSpace`: Confirms freed space is reallocated to new entries.
10. `Test_10_SearchForExistingVehicle`: Tests overloaded search by registration and slot number.
11. `Test_11_HandleNonexistentVehicleSearch`: Tests search queries for unparked vehicles.
12. `Test_12_HandleRemovalOfNonexistentVehicle`: Asserts `VehicleNotFoundException` on bad removals.
13. `Test_13_SaveAndReloadParkedVehiclesCorrectly`: Tests persistence round-trip.
14. `Test_14_PreserveVehicleTypeAndParkingSpaceAfterReload`: Tests that vehicle types and slot numbers remain intact across persistence reloads.
15. `Test_15_VerifyVehicleComparisonUsingOverloadedOperator`: Verifies `operator==` compares vehicles by registration.
16. `Test_16_VerifyVehicleCountsBehaveAsDocumented`: Verifies distinction between lifetime object count vs. active parked count.
17. `Test_17_VerifyFailedParkingOperationsDoNotCorruptRecords`: Verifies Strong Exception Guarantee (failed operations leave existing lot state intact).

---

## 10. Sample Program Interaction

```text
==================================================
   PARKING LOT MANAGEMENT SYSTEM (OOP in C++17)   
==================================================
[Initialization] Loading existing parking records from disk...
[Initialization] 0 vehicle(s) restored from storage.

================== MAIN MENU ==================
1. Park a Vehicle
2. Display All Parked Vehicles
3. Search Vehicle
4. Remove Vehicle and Calculate Fee
5. Show Available Parking Spaces
6. Show Parking Statistics
7. Save Data
8. Exit
===============================================
Enter your choice (1-8): 1

--- PARK A VEHICLE ---
Select Vehicle Type:
1. Car (Rate: Rs. 50 / hr)
2. Bike (Rate: Rs. 20 / hr)
Enter choice (1-2): 1
Enter Vehicle Registration Number: MH-12-DE-1234
Enter Owner Name: Rahul
Enter Estimated Parking Duration (hours): 3

[Success] Vehicle successfully parked!

========================================
            PARKING TICKET              
========================================
 Ticket ID       : TKT-1001
 Assigned Space  : Slot #1
 Registration No : MH-12-DE-1234
 Owner Name      : Rahul
 Vehicle Type    : Car
 Parking Duration: 3 hrs
 Estimated Fee   : Rs. 150.00
 Entry Time      : 2026-10-16 17:56:47
========================================
```

---

## 11. Known Limitations & Future Enhancements

- **Flat Hourly Billing**: Currently calculates billing using full hourly units. Future versions could support minute-level granularity.
- **Fixed Capacity**: Default lot capacity is 10 spaces (configurable via constructor). A dynamic multi-level parking lot could be supported.
- **Vehicle Dimensions**: Assumes single slot per vehicle. In the future, large vehicles (trucks, buses) could span multiple consecutive slots.
