# 🚗 ParkEasy – Parking Lot Management System

**ParkEasy** is a C++ console-based Parking Lot Management System designed to manage vehicle parking records efficiently. It helps users store vehicle information, search for vehicles, display parking details, and calculate parking charges.

This project is developed using **Object-Oriented Programming (OOP) concepts in C++**.

## 📌 Project Overview

Managing vehicle parking manually can be time-consuming and difficult. ParkEasy provides a simple, menu-driven solution to organize parking information and calculate parking fees based on the parking duration.

The project demonstrates how C++ programming concepts can be applied to a real-life parking management application.

## ✨ Features

- 🚘 Store vehicle details, including vehicle number and owner name.
- 🔍 Search for vehicles using their vehicle number.
- 📋 Display vehicle parking information.
- 💰 Calculate parking charges according to the implemented charging rules.
- 🔢 Maintain a count of vehicles using a static data member.
- 💾 Use file handling to store or retrieve records, where implemented.
- 🧩 Apply object-oriented programming concepts.
- 🖥️ Provide a simple, menu-driven console interface.

## 🛠️ Technologies Used

- **Programming Language:** C++
- **Concepts:** Object-Oriented Programming (OOP)
- **Libraries:** `iostream`, `fstream`, `iomanip`, `string`
- **Compiler:** GCC, MinGW, or another compatible C++ compiler
- **Platform:** Windows, Linux, or any platform with a compatible C++ compiler

## 📚 OOP Concepts Implemented

| Concept | Purpose |
|---|---|
| Classes and Objects | Organize vehicle data and operations |
| Encapsulation | Group data and related functions |
| Inheritance | Extend the functionality of the base `Vehicle` class |
| Abstraction | Define essential vehicle operations through an abstract class |
| Polymorphism | Use virtual functions to support different implementations |
| Constructor | Initialize vehicle details |
| Static Data Member | Maintain a shared vehicle counter |
| Function Overloading | Provide multiple ways to search for vehicles |
| File Handling | Store and retrieve data if supported by the program |

## 📂 Project Structure

```text
parkeasy-parking-lot-system/
│
├── main.cpp       # Main C++ source code
└── README.md      # Project documentation
```

*Note: Update the file names above if your repository uses different names.*

## ⚙️ How to Run the Project

### Step 1: Download the project

Clone the repository using Git:

```bash
git clone https://github.com/harmitdhaduk-04/parkeasy-parking-lot-system.git
```

### Step 2: Open the project folder

```bash
cd parkeasy-parking-lot-system
```

### Step 3: Compile the program

If your main source file is named `main.cpp`, run:

```bash
g++ main.cpp -o parkeasy
```

### Step 4: Run the program

**Windows:**

```bash
parkeasy.exe
```

**Linux:**

```bash
./parkeasy
```

You can also open the source file in an IDE such as Visual Studio Code, Code::Blocks, or another C++ development environment and compile and run it there.

## 🖥️ How to Use

1. Run the compiled program.
2. Select an operation from the displayed menu.
3. Enter the required vehicle information.
4. Search for a vehicle or display the available parking details.
5. Calculate the parking charge using the implemented rules.
6. Follow the menu instructions to continue or exit.

*The available operations depend on the functions implemented in the source code.*

## 🎯 Project Objectives

- To develop a basic parking management application using C++.
- To understand and implement important OOP concepts.
- To practice constructors, inheritance, polymorphism, and function overloading.
- To learn how file handling can be used for data storage.
- To apply programming concepts to a practical, real-world problem.

## 🚀 Future Improvements

- Add a graphical user interface (GUI).
- Track available and occupied parking spaces.
- Record vehicle entry and exit times automatically.
- Add different parking rates for different vehicle types.
- Generate parking receipts.
- Improve record management and validation.

## 👨‍💻 Author

**Harmit Dhaduk**

GitHub: [@harmitdhaduk-04](https://github.com/harmitdhaduk-04)

## 📄 License

This project is intended for educational and learning purposes. Add an appropriate open-source license if you want others to reuse, modify, and distribute the code under specified terms.

---

⭐ If you find this project useful, consider giving the repository a star!
