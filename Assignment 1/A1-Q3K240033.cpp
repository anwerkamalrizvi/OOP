#include <iostream>
#include <string>

using namespace std;

class User 
{
public:
    int person_id, age;
    string name, lic_type, contact;
    User(int id, string naam, int umar, string kakhzaad, string rabta) : person_id(id), name(naam), age(umar), lic_type(kakhzaad), contact(rabta) {}
  
    void update_details(string naam, int umar, string kakhzaad, string rabta) 

    {
        name = naam;
        age = umar;
        lic_type =kakhzaad;
        contact = rabta;
    }
};

class Vehicle 

{
public:
    string model;
	string lic_req_car;
    double rent_price_car;

    Vehicle(string style, double qeemat, string kakhzaad) : model(style), rent_price_car(qeemat), lic_req_car(kakhzaad) {}
    bool is_valid(string kakhzaad){
        return kakhzaad == lic_req_car; 
    }
};

class RentalSystem 
{
public:

    User* users[100];
    Vehicle* vehicles[50];
    int counter_user, counter_veh;
    RentalSystem() : counter_user(0), counter_veh(0) {}
    void registering_user(int id, string naam, int umar, string kakhzaad, string raabta) 
    {
        users[counter_user++] = new User (id, naam, umar, kakhzaad, raabta);
    }

    void adding_new_vehicle(string style, double qeemat, string kakhzaad) 
    {
        vehicles[counter_veh++] = new Vehicle(style, qeemat, kakhzaad);
    }

    void renting_vehicle(int person_id, string style) 
    {

        for (int i = 0; i < counter_user; i++) 
        {
            if (users[i]->person_id == person_id) 
            {
                for (int j = 0; j < counter_veh; j++) 
                {
                    if (vehicles[j]->model == style) 
                    {
                        if (vehicles[j]->is_valid(users[i]->lic_type)) 
                        {
                            cout << users[i]->name << " rented " << vehicles[j]->model << " for Rupees " << vehicles[j]->rent_price_car << " per day." << endl;
                        } else 
                        {
                            cout << "userr is not qualified to rent this vehicle." << endl;
                        }
                        return;
                    }
                }
            }
        }
        cout << "user OR vehicle not found." << endl;
    }
};

int main() 
{
    RentalSystem s1;
    s1.registering_user(1, "Anwer Kamal Rizvi",21, "Learner", "9269420");  s1.adding_new_vehicle("Toyota Axio 2018", 40000, "Full");
    s1.adding_new_vehicle("Honda Vezel 2016", 200, "Learner");
	s1.renting_vehicle(1, "Honda Vezel 2016");  
    
	s1.renting_vehicle(1, "Toyota Axio 2018");
	s1.renting_vehicle(1, "Corolla GLI 2017");

    return 0;
}

