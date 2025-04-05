#include <iostream>
using namespace std;
class User 
{
public:
    string userName;
    int userId;
    User(string uname, int uid) : userName(uname), userId(uid) {}
    virtual void payment() = 0;
    virtual ~User() {}
};

class Student : public User 
{
public:
    double studentFee;
    Student(string uname, int uid, double fee) : User(uname, uid), studentFee(fee) {}
    void payment() override 
    {
        cout << userName << " pay rupees " << studentFee << " per semester for transport." << endl;
    }
};

class Teacher : public User 
{
public:
    double teacherFee;
    Teacher(string uname, int uid, double fee) : User(uname, uid), teacherFee(fee) {}
    void payment() override 
    {
        cout << userName << " pay rupees " << teacherFee << " per month for transport." << endl;
    }
};

class Staff : public User 
{
public:
    double staffFee;
    Staff(string uname, int uid, double fee) : User(uname, uid), staffFee(fee) {}
    void payment() override 
    {
        cout << userName << " pays " << staffFee << " per month for transport." << endl;
    }
};

class Route 
{
public:
    string routeTitle;
    int routeCode;
    string routeStops[10];
    Route(string title, int code, string stops[10]) : routeTitle(title), routeCode(code) 
    {
        for (int i = 0; i < 10; i++) 
            routeStops[i] = stops[i];
    }
    bool operator == (const Route &r) 
    {
        return routeCode == r.routeCode;
    }
    void display() 
    {
        cout << "( " << routeTitle  << " ) -" << " route number: " << routeCode << endl;
    }
};

class Bus 
{
public:
    int busNumber;
    Route* assignedRoute;
    Bus(int number, Route* routePtr) : busNumber(number), assignedRoute(routePtr) {}
    void displayer_for_bus() 
    {
        cout << endl << "bus id: " << busNumber << " assigned to route: "; assignedRoute->display();
    }
};

class TransportSystem 
{
public:
    User* allUsers[100];
    Bus* allBuses[50];
    Route* allRoutes[20];
    int userCount, busCount, routeCount;
    
    TransportSystem() : userCount(0), busCount(0), routeCount(0) {}

    void add_user(User* usr) 
    { 
        allUsers[userCount++] = usr; 
    }

    void add_route(Route* rt) 
    { 
        allRoutes[routeCount++] = rt; 
    }

    void add_bus(Bus* bus) 
    {
        allBuses[busCount++] = bus; 
    }

    void process_payment() 
    {
        for (int i = 0; i < userCount; i++) 
            allUsers[i]->payment();
    }
};

int main() 
{
    TransportSystem ts;
    string defaultStops[10] = {"Fast", "Landhi", "Quaidabad", "Malir", "Drigh Road", "PECHS", "PIDC", "Netty Jetty Bridge", "Karachi Port - Terminal 1", "Karachi Port - Terminal 2"};
    Route route1("Main Campus - City Center", 1, defaultStops);
    ts.add_route(&route1);
    Teacher teacher1("Prof: Nazeer", 156, 10000);
    Student student1("Anwer", 240033, 50000);
    Staff staff1("Kashif", 333, 9000);
    ts.add_user(&student1);
    ts.add_user(&teacher1);
    ts.add_user(&staff1);
    Bus bus1(1, &route1);
    ts.add_bus(&bus1);
    ts.process_payment();
    bus1.displayer_for_bus();
    return 0;
}
