#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Custom exception classes without stdexcept
class EntityNotFoundException {
    string msg;
public:
    EntityNotFoundException(const string& message) : msg(message) {}
    const char* what() const { return msg.c_str(); }
};

class BookingException {
    string msg;
public:
    BookingException(const string& message) : msg(message) {}
    const char* what() const { return msg.c_str(); }
};

// Forward declarations
class Route;
class Driver;
class Vehicle;
class Booking;
class User;

// Generic List class without smart pointers
template <typename T>
class GenericList {
    vector<T*> items;
public:
    ~GenericList() {
        for (auto item : items) {
            delete item;
        }
    }

    void add(T* item) {
        items.push_back(item);
    }

    T* getById(const string& id) {
        for (auto item : items) {
            if (item->getId() == id) {
                return item;
            }
        }
        throw EntityNotFoundException("Item with ID " + id + " not found");
    }

    vector<T*> getAll() const {
        return items;
    }

    void remove(const string& id) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->getId() == id) {
                delete *it;
                items.erase(it);
                return;
            }
        }
        throw EntityNotFoundException("Item with ID " + id + " not found");
    }
};

// Simple time handling without ctime
struct SimpleDate {
    int year, month, day;
    
    SimpleDate() {
        // Very simple "current date" implementation
        year = 2023;
        month = 11;
        day = 1;
    }
    
    string toString() const {
        return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    }
};

// Route class
class Route {
    string routeId;
    string startLocation;
    string endLocation;
    float distance;
    bool isLongDistance;
    float longDistanceThreshold;

public:
    Route(const string& id, const string& start, const string& end, float dist, float threshold = 30.0f)
        : routeId(id), startLocation(start), endLocation(end), distance(dist), longDistanceThreshold(threshold) {
        isLongDistance = (distance > longDistanceThreshold);
    }

    string getId() const { return routeId; }
    string getStartLocation() const { return startLocation; }
    string getEndLocation() const { return endLocation; }
    float getDistance() const { return distance; }
    bool getIsLongDistance() const { return isLongDistance; }

    string toString() const {
        return "Route " + routeId + ": " + startLocation + " to " + endLocation + 
               " (" + to_string(distance) + " km)" + (isLongDistance ? " [Long Distance]" : "");
    }
};

// Driver class
class Driver {
    string driverId;
    string name;
    string licenseNumber;
    Vehicle* assignedVehicle;

public:
    Driver(const string& id, const string& name, const string& license)
        : driverId(id), name(name), licenseNumber(license), assignedVehicle(nullptr) {}

    string getId() const { return driverId; }
    string getName() const { return name; }
    string getLicenseNumber() const { return licenseNumber; }
    Vehicle* getAssignedVehicle() const { return assignedVehicle; }

    void assignVehicle(Vehicle* vehicle);
    void removeAssignment();
    string toString() const;  // Declaration only
};

// User hierarchy
class User {
protected:
    string userId;
    string name;
    string email;
    bool paymentStatus;

public:
    User(const string& id, const string& name, const string& email)
        : userId(id), name(name), email(email), paymentStatus(false) {}

    virtual ~User() {}

    string getId() const { return userId; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    bool getPaymentStatus() const { return paymentStatus; }

    void makePayment() { paymentStatus = true; }
    virtual float calculateFare(bool isAC) const = 0;
    virtual string getUserType() const = 0;

    string toString() const {
        return getUserType() + " " + userId + ": " + name + " (" + email + ")" +
               " [Payment: " + (paymentStatus ? "Completed" : "Pending") + "]";
    }
};

class Student : public User {
    const float BASE_FARE = 1000.0f;
    const float AC_SURCHARGE = 2000.0f;

public:
    Student(const string& id, const string& name, const string& email)
        : User(id, name, email) {}

    float calculateFare(bool isAC) const override {
        return BASE_FARE + (isAC ? AC_SURCHARGE : 0);
    }

    string getUserType() const override { return "Student"; }
};

class Faculty : public User {
    const float BASE_FARE = 1500.0f;
    const float AC_SURCHARGE = 2000.0f;

public:
    Faculty(const string& id, const string& name, const string& email)
        : User(id, name, email) {}

    float calculateFare(bool isAC) const override {
        return BASE_FARE + (isAC ? AC_SURCHARGE : 0);
    }

    string getUserType() const override { return "Faculty"; }
};

// Vehicle hierarchy
class Vehicle {
protected:
    string vehicleId;
    string model;
    bool isAC;
    Driver* driver;
    Route* route;
    vector<bool> seats; // true = booked, false = available
    vector<string> seatUserIds; // user ID for each seat

public:
    Vehicle(const string& id, const string& model, bool ac, int totalSeats)
        : vehicleId(id), model(model), isAC(ac), seats(totalSeats, false), 
          seatUserIds(totalSeats, ""), driver(nullptr), route(nullptr) {}

    virtual ~Vehicle() {}

    string getId() const { return vehicleId; }
    string getModel() const { return model; }
    bool getIsAC() const { return isAC; }
    Driver* getDriver() const { return driver; }
    Route* getRoute() const { return route; }
    int getTotalSeats() const { return seats.size(); }
    int getAvailableSeats() const {
        int count = 0;
        for (bool booked : seats) {
            if (!booked) count++;
        }
        return count;
    }

    void assignDriver(Driver* newDriver) {
        if (driver) {
            driver->removeAssignment();
        }
        driver = newDriver;
        if (newDriver) {
            newDriver->assignVehicle(this);
        }
    }

    void assignRoute(Route* newRoute) {
        route = newRoute;
    }

    virtual string getVehicleType() const = 0;

    int bookSeat(const string& userId, bool isFaculty) {
        for (int i = 0; i < seats.size(); ++i) {
            if (!seats[i]) {
                // Check if this is a faculty seat (first 5 seats)
                if ((isFaculty && i < 5) || (!isFaculty && i >= 5)) {
                    seats[i] = true;
                    seatUserIds[i] = userId;
                    return i + 1; // Seat numbers start from 1
                }
            }
        }
        return -1; // No available seat
    }

    bool cancelSeat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > seats.size()) {
            return false;
        }
        if (seats[seatNumber - 1]) {
            seats[seatNumber - 1] = false;
            seatUserIds[seatNumber - 1] = "";
            return true;
        }
        return false;
    }

    string toString() const {
        string result = getVehicleType() + " " + vehicleId + ": " + model + 
                       " (" + (isAC ? "AC" : "Non-AC") + ")\n";
        result += "Seats: " + to_string(getAvailableSeats()) + "/" + to_string(seats.size()) + " available\n";
        if (driver) {
            result += "Driver: " + driver->getName() + "\n";
        }
        if (route) {
            result += "Route: " + route->getStartLocation() + " to " + route->getEndLocation() + "\n";
        }
        return result;
    }
};

class Bus : public Vehicle {
public:
    Bus(const string& id, const string& model, bool ac)
        : Vehicle(id, model, ac, 52) {}

    string getVehicleType() const override { return "Bus"; }
};

class Coaster : public Vehicle {
public:
    Coaster(const string& id, const string& model, bool ac)
        : Vehicle(id, model, ac, 32) {}

    string getVehicleType() const override { return "Coaster"; }
};

// Complete Driver methods now that Vehicle is defined
void Driver::assignVehicle(Vehicle* vehicle) {
    if (assignedVehicle && assignedVehicle != vehicle) {
        assignedVehicle->assignDriver(nullptr);
    }
    assignedVehicle = vehicle;
    if (vehicle && vehicle->getDriver() != this) {
        vehicle->assignDriver(this);
    }
}

void Driver::removeAssignment() {
    if (assignedVehicle) {
        auto temp = assignedVehicle;
        assignedVehicle = nullptr;
        temp->assignDriver(nullptr);
    }
}

string Driver::toString() const {
    return "Driver " + driverId + ": " + name + " (License: " + licenseNumber + ")" +
           (assignedVehicle ? " [Assigned to Vehicle " + assignedVehicle->getId() + "]" : " [Unassigned]");
}

// Booking class
class Booking {
    string bookingId;
    SimpleDate bookingDate;
    User* user;
    Vehicle* vehicle;
    Route* route;
    int seatNumber;
    float fare;

public:
    Booking(const string& id, User* user, Vehicle* vehicle, int seatNumber)
        : bookingId(id), user(user), vehicle(vehicle), seatNumber(seatNumber) {
        route = vehicle->getRoute();
        fare = user->calculateFare(vehicle->getIsAC());
    }

    string getId() const { return bookingId; }
    SimpleDate getBookingDate() const { return bookingDate; }
    User* getUser() const { return user; }
    Vehicle* getVehicle() const { return vehicle; }
    Route* getRoute() const { return route; }
    int getSeatNumber() const { return seatNumber; }
    float getFare() const { return fare; }

    string toString() const {
        return "Booking " + bookingId + " on " + bookingDate.toString() + "\n" +
               "User: " + user->getName() + "\n" +
               "Vehicle: " + vehicle->getVehicleType() + " " + vehicle->getId() + "\n" +
               "Route: " + route->getStartLocation() + " to " + route->getEndLocation() + "\n" +
               "Seat: " + to_string(seatNumber) + "\n" +
               "Fare: Rs. " + to_string(fare);
    }
};

// Transporter class (abstract)
class Transporter {
protected:
    string name;
    GenericList<Vehicle> vehicles;
    GenericList<Driver> drivers;
    GenericList<Route> routes;

public:
    Transporter(const string& name) : name(name) {}
    virtual ~Transporter() {}

    string getName() const { return name; }

    void addVehicle(Vehicle* vehicle) {
        vehicles.add(vehicle);
    }

    void addDriver(Driver* driver) {
        drivers.add(driver);
    }

    void addRoute(Route* route) {
        routes.add(route);
    }

    Vehicle* getVehicle(const string& id) {
        return vehicles.getById(id);
    }

    Driver* getDriver(const string& id) {
        return drivers.getById(id);
    }

    Route* getRoute(const string& id) {
        return routes.getById(id);
    }

    vector<Vehicle*> getAllVehicles() const {
        return vehicles.getAll();
    }

    vector<Driver*> getAllDrivers() const {
        return drivers.getAll();
    }

    vector<Route*> getAllRoutes() const {
        return routes.getAll();
    }

    virtual string getTransporterType() const = 0;
};

class NadeemTransporter : public Transporter {
public:
    NadeemTransporter() : Transporter("Nadeem Transporter") {}

    string getTransporterType() const override { return "Nadeem"; }
};

class ZulfiqarTransporter : public Transporter {
public:
    ZulfiqarTransporter() : Transporter("Zulfiqar Transporter") {}

    string getTransporterType() const override { return "Zulfiqar"; }
};

// Simple string splitting function to replace stringstream
vector<string> splitString(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    for (char ch : s) {
        if (ch == delimiter) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

// Transport Management System class
class TransportSystem {
    GenericList<User> users;
    GenericList<Booking> bookings;
    vector<Transporter*> transporters;

public:
    TransportSystem() {
        // Initialize with both transporters
        transporters.push_back(new NadeemTransporter());
        transporters.push_back(new ZulfiqarTransporter());
    }

    ~TransportSystem() {
        for (auto transporter : transporters) {
            delete transporter;
        }
    }

    void addUser(User* user) {
        users.add(user);
    }

    User* getUser(const string& id) {
        return users.getById(id);
    }

    Booking* bookSeat(const string& userId, const string& vehicleId) {
        User* user = users.getById(userId);
        if (!user->getPaymentStatus()) {
            throw BookingException("Payment not completed for user " + userId);
        }

        // Find vehicle in any transporter
        Vehicle* vehicle = nullptr;
        for (auto transporter : transporters) {
            try {
                vehicle = transporter->getVehicle(vehicleId);
                break;
            } catch (const EntityNotFoundException&) {
                continue;
            }
        }

        if (!vehicle) {
            throw EntityNotFoundException("Vehicle with ID " + vehicleId + " not found");
        }

        if (!vehicle->getRoute()) {
            throw BookingException("Vehicle " + vehicleId + " has no assigned route");
        }

        bool isFaculty = dynamic_cast<Faculty*>(user) != nullptr;
        int seatNumber = vehicle->bookSeat(userId, isFaculty);
        if (seatNumber == -1) {
            throw BookingException(string("No available seats for ") + 
                                 (isFaculty ? "faculty" : "student") + 
                                 " in vehicle " + vehicleId);
        }

        string bookingId = "B" + to_string(bookings.getAll().size() + 1000);
        Booking* booking = new Booking(bookingId, user, vehicle, seatNumber);
        bookings.add(booking);
        return booking;
    }

    void saveToFiles() const {
        // Save users
        ofstream userFile("users.txt");
        for (auto user : users.getAll()) {
            userFile << user->getUserType() << ","
                     << user->getId() << ","
                     << user->getName() << ","
                     << user->getEmail() << ","
                     << user->getPaymentStatus() << "\n";
        }
        userFile.close();

        // Save bookings
        ofstream bookingFile("bookings.txt");
        for (auto booking : bookings.getAll()) {
            bookingFile << booking->getId() << ","
                       << booking->getUser()->getId() << ","
                       << booking->getVehicle()->getId() << ","
                       << booking->getSeatNumber() << ","
                       << booking->getFare() << ","
                       << booking->getBookingDate().toString() << "\n";
        }
        bookingFile.close();

        // Save vehicles, drivers, routes for each transporter
        for (auto transporter : transporters) {
            string prefix = transporter->getTransporterType();

            // Save vehicles
            ofstream vehicleFile(prefix + "_vehicles.txt");
            for (auto vehicle : transporter->getAllVehicles()) {
                vehicleFile << vehicle->getVehicleType() << ","
                           << vehicle->getId() << ","
                           << vehicle->getModel() << ","
                           << vehicle->getIsAC() << ","
                           << (vehicle->getDriver() ? vehicle->getDriver()->getId() : "") << ","
                           << (vehicle->getRoute() ? vehicle->getRoute()->getId() : "") << "\n";
            }
            vehicleFile.close();

            // Save drivers
            ofstream driverFile(prefix + "_drivers.txt");
            for (auto driver : transporter->getAllDrivers()) {
                driverFile << driver->getId() << ","
                          << driver->getName() << ","
                          << driver->getLicenseNumber() << "\n";
            }
            driverFile.close();

            // Save routes
            ofstream routeFile(prefix + "_routes.txt");
            for (auto route : transporter->getAllRoutes()) {
                routeFile << route->getId() << ","
                          << route->getStartLocation() << ","
                          << route->getEndLocation() << ","
                          << route->getDistance() << "\n";
            }
            routeFile.close();
        }
    }

    void loadFromFiles() {
        // Load users
        ifstream userFile("users.txt");
        if (userFile) {
            string line;
            while (getline(userFile, line)) {
                vector<string> parts = splitString(line, ',');
                if (parts.size() < 5) continue;

                User* user;
                if (parts[0] == "Student") {
                    user = new Student(parts[1], parts[2], parts[3]);
                } else {
                    user = new Faculty(parts[1], parts[2], parts[3]);
                }

                if (parts[4] == "1") {
                    user->makePayment();
                }
                users.add(user);
            }
            userFile.close();
        }

        // Load transporters' data
        for (auto transporter : transporters) {
            string prefix = transporter->getTransporterType();

            // Load drivers first
            ifstream driverFile(prefix + "_drivers.txt");
            if (driverFile) {
                string line;
                while (getline(driverFile, line)) {
                    vector<string> parts = splitString(line, ',');
                    if (parts.size() < 3) continue;
                    transporter->addDriver(new Driver(parts[0], parts[1], parts[2]));
                }
                driverFile.close();
            }

            // Load routes
            ifstream routeFile(prefix + "_routes.txt");
            if (routeFile) {
                string line;
                while (getline(routeFile, line)) {
                    vector<string> parts = splitString(line, ',');
                    if (parts.size() < 4) continue;
                    float distance = stof(parts[3]);
                    transporter->addRoute(new Route(parts[0], parts[1], parts[2], distance));
                }
                routeFile.close();
            }

            // Load vehicles
            ifstream vehicleFile(prefix + "_vehicles.txt");
            if (vehicleFile) {
                string line;
                while (getline(vehicleFile, line)) {
                    vector<string> parts = splitString(line, ',');
                    if (parts.size() < 6) continue;
                    
                    bool ac = (parts[3] == "1");
                    Vehicle* vehicle;
                    if (parts[0] == "Bus") {
                        vehicle = new Bus(parts[1], parts[2], ac);
                    } else {
                        vehicle = new Coaster(parts[1], parts[2], ac);
                    }

                    if (!parts[4].empty()) {
                        try {
                            Driver* driver = transporter->getDriver(parts[4]);
                            vehicle->assignDriver(driver);
                        } catch (const EntityNotFoundException&) {
                            cerr << "Driver " << parts[4] << " not found for vehicle " << parts[1] << endl;
                        }
                    }

                    if (!parts[5].empty()) {
                        try {
                            Route* route = transporter->getRoute(parts[5]);
                            vehicle->assignRoute(route);
                        } catch (const EntityNotFoundException&) {
                            cerr << "Route " << parts[5] << " not found for vehicle " << parts[1] << endl;
                        }
                    }

                    transporter->addVehicle(vehicle);
                }
                vehicleFile.close();
            }
        }

        // Load bookings
        ifstream bookingFile("bookings.txt");
        if (bookingFile) {
            string line;
            while (getline(bookingFile, line)) {
                vector<string> parts = splitString(line, ',');
                if (parts.size() < 6) continue;

                try {
                    User* user = users.getById(parts[1]);
                    
                    // Find vehicle in any transporter
                    Vehicle* vehicle = nullptr;
                    for (auto transporter : transporters) {
                        try {
                            vehicle = transporter->getVehicle(parts[2]);
                            break;
                        } catch (const EntityNotFoundException&) {
                            continue;
                        }
                    }

                    if (!vehicle) {
                        cerr << "Vehicle " << parts[2] << " not found for booking " << parts[0] << endl;
                        continue;
                    }

                    int seatNumber = stoi(parts[3]);
                    float fare = stof(parts[4]);
                    // Simple date parsing - in a real system you'd want better parsing
                    SimpleDate date;

                    // Mark the seat as booked
                    bool isFaculty = dynamic_cast<Faculty*>(user) != nullptr;
                    if (seatNumber > 0 && seatNumber <= vehicle->getTotalSeats()) {
                        vehicle->bookSeat(parts[1], isFaculty);
                    }

                    Booking* booking = new Booking(parts[0], user, vehicle, seatNumber);
                    bookings.add(booking);
                } catch (const EntityNotFoundException& e) {
                    cerr << "Error loading booking " << parts[0] << ": " << e.what() << endl;
                }
            }
            bookingFile.close();
        }
    }

    const vector<Transporter*>& getTransporters() const {
        return transporters;
    }
};

// Example usage
int main() {
    TransportSystem system;

    // Load data from files if they exist
    system.loadFromFiles();

    // Example of creating new entities
    Student* student1 = new Student("S1001", "Ali Khan", "ali@nu.edu.pk");
    student1->makePayment();
    system.addUser(student1);

    Faculty* faculty1 = new Faculty("F2001", "Dr. Ahmed", "ahmed@nu.edu.pk");
    faculty1->makePayment();
    system.addUser(faculty1);

    // Get the first transporter (Nadeem)
    auto transporter = system.getTransporters()[0];

    // Add a driver
    Driver* driver1 = new Driver("D1001", "Nadeem Ahmed", "LIC12345");
    transporter->addDriver(driver1);

    // Add a route
    Route* route1 = new Route("R1001", "Faisal Town", "FAST-NUCES", 15.5f);
    transporter->addRoute(route1);

    // Add a vehicle
    Bus* bus1 = new Bus("V1001", "Toyota Coaster", true);
    bus1->assignDriver(driver1);
    bus1->assignRoute(route1);
    transporter->addVehicle(bus1);

    // Make a booking
    try {
        Booking* booking = system.bookSeat("S1001", "V1001");
        cout << "Booking successful:\n" << booking->toString() << endl;
    } catch (const exception& e) {
        cerr << "Booking failed: " << e.what() << endl;
    }

    // Save all data to files
    system.saveToFiles();

    return 0;
}