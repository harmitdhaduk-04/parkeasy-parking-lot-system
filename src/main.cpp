#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// ================= VEHICLE CLASS =================

class Vehicle
{
protected:
    string number;
    string owner;
    int hours;

public:
    // Static data member
    static int totalVehicles;

    // Constructor
    Vehicle(string n = "", string o = "", int h = 0)
    {
        this->number = n;
        this->owner = o;
        this->hours = h;
    }

    // Virtual function
    virtual void display()
    {
        cout << "Vehicle Number : " << number << endl;
        cout << "Owner Name     : " << owner << endl;
        cout << "Parking Hours  : " << hours << endl;
    }

    // Pure virtual function
    virtual double calculateCharge() = 0;

    // Getter
    string getNumber()
    {
        return number;
    }

    // Function overloading
    void searchVehicle(string n)
    {
        if (number == n)
            cout << "Vehicle Found!" << endl;
    }

    void searchVehicle()
    {
        cout << "Vehicle Number: " << number << endl;
    }

    // Operator overloading
    bool operator==(Vehicle &v)
    {
        return number == v.number;
    }

    // Destructor
    virtual ~Vehicle()
    {
    }

    // Friend function
    friend void showTotalVehicles();
};

// Initialize static variable
int Vehicle::totalVehicles = 0;


// ================= CAR CLASS =================

class Car : public Vehicle
{
public:

    Car(string n = "", string o = "", int h = 0)
        : Vehicle(n, o, h)
    {
        totalVehicles++;
    }

    // Function overriding
    void display() override
    {
        cout << "\n--- CAR DETAILS ---" << endl;
        Vehicle::display();
        cout << "Vehicle Type   : Car" << endl;
        cout << "Parking Charge : Rs. "
             << calculateCharge() << endl;
    }

    double calculateCharge() override
    {
        return hours * 50;
    }
};


// ================= BIKE CLASS =================

class Bike : public Vehicle
{
public:

    Bike(string n = "", string o = "", int h = 0)
        : Vehicle(n, o, h)
    {
        totalVehicles++;
    }

    void display() override
    {
        cout << "\n--- BIKE DETAILS ---" << endl;
        Vehicle::display();
        cout << "Vehicle Type   : Bike" << endl;
        cout << "Parking Charge : Rs. "
             << calculateCharge() << endl;
    }

    double calculateCharge() override
    {
        return hours * 20;
    }
};


// ================= FRIEND FUNCTION =================

void showTotalVehicles()
{
    cout << "\nTotal Vehicles Created: "
         << Vehicle::totalVehicles << endl;
}


// ================= TEMPLATE FUNCTION =================

template <class T>
void showMessage(T message)
{
    cout << message << endl;
}


// ================= PARKING LOT CLASS =================

class ParkingLot
{
private:

    Vehicle *vehicles[10];

    int count;

public:

    // Constructor
    ParkingLot()
    {
        count = 0;

        for (int i = 0; i < 10; i++)
            vehicles[i] = nullptr;
    }

    // Add vehicle
    void addVehicle()
    {
        if (count >= 10)
        {
            throw "Parking Lot is Full!";
        }

        int type;
        string number, owner;
        int hours;

        cout << "\nEnter Vehicle Type:" << endl;
        cout << "1. Car" << endl;
        cout << "2. Bike" << endl;
        cout << "Enter choice: ";
        cin >> type;

        cout << "Enter Vehicle Number: ";
        cin >> number;

        cout << "Enter Owner Name: ";
        cin >> owner;

        cout << "Enter Parking Hours: ";
        cin >> hours;

        if (hours <= 0)
        {
            throw "Invalid parking hours!";
        }

        if (type == 1)
        {
            vehicles[count] =
                new Car(number, owner, hours);
        }
        else if (type == 2)
        {
            vehicles[count] =
                new Bike(number, owner, hours);
        }
        else
        {
            throw "Invalid Vehicle Type!";
        }

        count++;

        cout << "\nVehicle parked successfully!" << endl;
    }


    // Display all vehicles
    void displayVehicles()
    {
        if (count == 0)
        {
            cout << "\nNo vehicles in parking lot." << endl;
            return;
        }

        cout << "\n========== PARKED VEHICLES ==========\n";

        for (int i = 0; i < count; i++)
        {
            vehicles[i]->display();
            cout << "-----------------------------------\n";
        }
    }


    // Search vehicle
    void searchVehicle()
    {
        string number;

        cout << "\nEnter vehicle number to search: ";
        cin >> number;

        bool found = false;

        for (int i = 0; i < count; i++)
        {
            if (vehicles[i]->getNumber() == number)
            {
                cout << "\nVehicle Found!" << endl;
                vehicles[i]->display();
                found = true;
                break;
            }
        }

        if (!found)
            cout << "\nVehicle not found." << endl;
    }


    // Remove vehicle
    void removeVehicle()
    {
        string number;

        cout << "\nEnter vehicle number to remove: ";
        cin >> number;

        for (int i = 0; i < count; i++)
        {
            if (vehicles[i]->getNumber() == number)
            {
                delete vehicles[i];

                for (int j = i; j < count - 1; j++)
                {
                    vehicles[j] = vehicles[j + 1];
                }

                vehicles[count - 1] = nullptr;

                count--;

                cout << "\nVehicle removed successfully!" << endl;
                return;
            }
        }

        cout << "\nVehicle not found." << endl;
    }


    // Save data into file
    void saveToFile()
    {
        ofstream file("parking.txt");

        if (!file)
        {
            cout << "File could not be opened!" << endl;
            return;
        }

        for (int i = 0; i < count; i++)
        {
            file << vehicles[i]->getNumber()
                 << endl;
        }

        file.close();

        cout << "\nParking data saved to file." << endl;
    }


    // Destructor
    ~ParkingLot()
    {
        for (int i = 0; i < count; i++)
        {
            delete vehicles[i];
        }
    }
};


// ================= MAIN FUNCTION =================

int main()
{
    ParkingLot parking;

    int choice;

    showMessage("=================================");
    showMessage("   PARKING LOT MANAGEMENT SYSTEM");
    showMessage("=================================");

    do
    {
        cout << "\n\n========== MENU ==========" << endl;

        cout << "1. Park Vehicle" << endl;
        cout << "2. Display Vehicles" << endl;
        cout << "3. Search Vehicle" << endl;
        cout << "4. Remove Vehicle" << endl;
        cout << "5. Save Data to File" << endl;
        cout << "6. Show Total Vehicles" << endl;
        cout << "7. Exit" << endl;

        cout << "\nEnter your choice: ";
        cin >> choice;

        try
        {
            switch (choice)
            {
            case 1:
                parking.addVehicle();
                break;

            case 2:
                parking.displayVehicles();
                break;

            case 3:
                parking.searchVehicle();
                break;

            case 4:
                parking.removeVehicle();
                break;

            case 5:
                parking.saveToFile();
                break;

            case 6:
                showTotalVehicles();
                break;

            case 7:
                cout << "\nThank you!" << endl;
                break;

            default:
                cout << "\nInvalid choice!" << endl;
            }
        }

        catch (const char *msg)
        {
            cout << "\nError: " << msg << endl;
        }

    } while (choice != 7);

    return 0;
}