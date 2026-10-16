cmake_minimum_required(VERSION 3.10)
project(ParkingLotManagementSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable compiler warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Core library source files
set(CORE_SOURCES
    src/ParkingFeeCalculator.cpp
    src/Vehicle.cpp
    src/Car.cpp
    src/Bike.cpp
    src/ParkingTicket.cpp
    src/ParkingLot.cpp
    src/FileManager.cpp
)

# Core Library
add_library(ParkingLotLib STATIC ${CORE_SOURCES})
target_include_directories(ParkingLotLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Main Application Executable
add_executable(ParkingLotApp src/main.cpp)
target_include_directories(ParkingLotApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(ParkingLotApp PRIVATE ParkingLotLib)

# Automated Test Executable
add_executable(ParkingLotTests tests/ParkingLotTests.cpp)
target_include_directories(ParkingLotTests PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/tests)
target_link_libraries(ParkingLotTests PRIVATE ParkingLotLib)

# Enable CTest integration
enable_testing()
add_test(NAME AutomatedUnitTests COMMAND ParkingLotTests)
