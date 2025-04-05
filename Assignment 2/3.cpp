#include <iostream>
#include <string>
using namespace std;

class Vehicle {
protected:
    int id;
    static int total;
    float speed, cap, energy;
public:
    Vehicle(int i, float s, float c, float e) : id(i), speed(s), cap(c), energy(e) {
        total++;
    }
    virtual string route(string dest) = 0;
    virtual float eta(float dist) = 0;
    virtual void move() = 0;
    virtual void command(string cmd, string pkg) = 0;
    virtual void command(string cmd, string pkg, string urgency) = 0;
    virtual ~Vehicle() {
        total--;
    }
    friend bool operator==(const Vehicle& v1, const Vehicle& v2) {
        return (v1.speed == v2.speed && v1.cap == v2.cap && v1.energy == v2.energy);
    }
    friend string resolve(const Vehicle& v1, const Vehicle& v2);
};

int Vehicle::total = 0;

string resolve(const Vehicle& v1, const Vehicle& v2) {
    float eff1 = v1.speed * v1.cap / v1.energy;
    float eff2 = v2.speed * v2.cap / v2.energy;
    return (eff1 > eff2) ? "Vehicle 1 chosen" : "Vehicle 2 chosen";
}

class RamzanDrone : public Vehicle {
public:
    RamzanDrone(int i) : Vehicle(i, 120, 5, 10) {}
    string route(string dest) override {
        return "Aerial path to " + dest;
    }
    float eta(float dist) override {
        return dist / speed;
    }
    void move() override {
        cout << "Drone flying with precision\n";
    }
    void command(string cmd, string pkg) override {
        cout << "Drone executing: " << cmd << " for " << pkg << endl;
    }
    void command(string cmd, string pkg, string urgency) override {
        if (urgency == "urgent")
            cout << "Drone in high-speed mode for " << pkg << endl;
        else
            command(cmd, pkg);
    }
};

class RamzanTimeShip : public Vehicle {
public:
    RamzanTimeShip(int i) : Vehicle(i, 300, 10, 30) {}
    string route(string dest) override {
        return "Temporal corridor to " + dest;
    }
    float eta(float dist) override {
        return (dist / speed) + 2;
    }
    void move() override {
        cout << "TimeShip traveling through timelines\n";
    }
    void command(string cmd, string pkg) override {
        cout << "TimeShip executing: " << cmd << " for " << pkg << endl;
    }
    void command(string cmd, string pkg, string urgency) override {
        if (urgency == "urgent")
            cout << "Validating history before sending " << pkg << endl;
        else
            command(cmd, pkg);
    }
};

class RamzanHyperPod : public Vehicle {
public:
    RamzanHyperPod(int i) : Vehicle(i, 80, 50, 20) {}
    string route(string dest) override {
        return "Underground tunnel to " + dest;
    }
    float eta(float dist) override {
        return dist / speed;
    }
    void move() override {
        cout << "HyperPod rolling fast underground\n";
    }
    void command(string cmd, string pkg) override {
        cout << "HyperPod executing: " << cmd << " for " << pkg << endl;
    }
    void command(string cmd, string pkg, string urgency) override {
        cout << "HyperPod processing urgent: " << pkg << endl;
    }
};

int main() {
    RamzanDrone d1(1);
    RamzanTimeShip t1(2);
    RamzanHyperPod h1(3);

    d1.command("Deliver", "PKG1", "urgent");
    t1.command("Deliver", "PKG2", "urgent");
    h1.command("Deliver", "PKG3", "normal");

    d1.move();
    t1.move();
    h1.move();

    cout << resolve(d1, t1) << endl;
    cout << (h1 == d1 ? "Same Efficiency" : "Different Efficiency") << endl;

    return 0;
}