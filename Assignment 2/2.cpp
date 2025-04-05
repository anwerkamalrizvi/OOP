#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;
class Ghost;
class BasicGhost;

class Ghost {
protected:
    string actor;
    int scare;
public:
    Ghost(string a) : actor(a) {
        scare = rand() % 10 + 1;
    }

    virtual void haunt() const = 0;
    virtual Ghost* clone() const = 0;
    int getScare() const { return scare; }
    string getActor() const { return actor; }

    friend ostream& operator<<(ostream& out, const Ghost& g) {
        out << "Ghost by " << g.actor << " | Scare: " << g.scare;
        return out;
    }

    Ghost* operator+(const Ghost& g);
    virtual ~Ghost() {}
};

class BasicGhost : public Ghost {
public:
    BasicGhost(string a, int s) : Ghost(a) { scare = s; }
    void haunt() const override {
        cout << actor << " makes eerie noises..." << endl;
    }
    Ghost* clone() const override {
        return new BasicGhost(*this);
    }
};

Ghost* Ghost::operator+(const Ghost& g) {
    string both = actor + " & " + g.actor;
    int total = scare + g.scare;
    return new BasicGhost(both, total);
}

class Poltergeist : public Ghost {
public:
    Poltergeist(string a) : Ghost(a) {}
    void haunt() const override {
        cout << actor << " hurls objects violently!" << endl;
    }
    Ghost* clone() const override {
        return new Poltergeist(*this);
    }
};

class Banshee : public Ghost {
public:
    Banshee(string a) : Ghost(a) {}
    void haunt() const override {
        cout << actor << " screams with piercing sound!" << endl;
    }
    Ghost* clone() const override {
        return new Banshee(*this);
    }
};

class ShadowGhost : public Ghost {
public:
    ShadowGhost(string a) : Ghost(a) {}
    void haunt() const override {
        cout << actor << " whispers from shadows..." << endl;
    }
    Ghost* clone() const override {
        return new ShadowGhost(*this);
    }
};

class ShadowPoltergeist : public Ghost {
public:
    ShadowPoltergeist(string a) : Ghost(a) {}
    void haunt() const override {
        cout << actor << " whispers eerily and flings a chair!" << endl;
    }
    Ghost* clone() const override {
        return new ShadowPoltergeist(*this);
    }
};

class Visitor {
    string name;
    int brave;
public:
    Visitor(string n, int b) : name(n), brave(b) {}

    int getBrave() const { return brave; }

    string braveText() const {
        if (brave <= 4) return "Coward";
        else if (brave <= 7) return "Normal";
        return "Bold";
    }

    void react(int scare) const {
        if (scare < brave)
            cout << name << " laughs at the ghost!" << endl;
        else if (scare > brave)
            cout << name << " screams and bolts!" << endl;
        else
            cout << name << " freezes with shaky breath..." << endl;
    }

    string getName() const { return name; }
};

class HauntedHouse {
    string title;
    Ghost* list[10];
    int count;
public:
    HauntedHouse(string t) : title(t), count(0) {}

    void add(Ghost* g) {
        if (count < 10)
            list[count++] = g->clone();
    }

    int size() const { return count; }
    string getTitle() const { return title; }

    Ghost* get(int i) const { return list[i]; }

    ~HauntedHouse() {
        for (int i = 0; i < count; ++i)
            delete list[i];
    }
};

void visit(Visitor group[], int num, HauntedHouse& house) {
    cout << "\n== " << house.getTitle() << " ==" << endl;
    for (int i = 0; i < num; ++i) {
        cout << "\nVisitor: " << group[i].getName() << " (" << group[i].braveText() << ")" << endl;
        for (int j = 0; j < house.size(); ++j) {
            house.get(j)->haunt();
            group[i].react(house.get(j)->getScare());
        }
    }
}

// Main
int main() {
    srand(time(0));

    Visitor group[4] = {
        Visitor("Ali", 3),
        Visitor("Sara", 5),
        Visitor("Qasim", 7),
        Visitor("Zain", 9)
    };

    HauntedHouse h1("Spooky Bhool Bhulaiyaan");
    h1.add(new Poltergeist("Jamshed"));
    h1.add(new Banshee("Nida"));
    h1.add(new ShadowGhost("Alina"));

    HauntedHouse h2("Abandoned Garden");
    h2.add(new ShadowPoltergeist("Milinna"));
    h2.add(new Banshee("Chaudry sahib"));
    h2.add(new Poltergeist("Bunti"));

    HauntedHouse h3("Old Graveyard");
    Ghost* g1 = new ShadowGhost("Abhishek");
    Ghost* g2 = new Banshee("Laila");
    Ghost* g3 = *g1 + *g2;
    h3.add(g1);
    h3.add(g2);
    h3.add(g3);
    delete g1;
    delete g2;
    delete g3;

    visit(group, 4, h1);
    visit(group, 4, h2);
    visit(group, 4, h3);

    return 0;
}
