#include <iostream>
#include <string>
using namespace std;

int hashPassword(const string& pwd) {
    unsigned long h = 5381;
    for (char ch : pwd) h = (h * 33) + ch;
    return h;
}

class Member {
protected:
    string uname, mail;
    int uid, h_pwd;
    string perms[5];
    int p_size;
public:
    Member(string n, int id, string m, string pwd, string p[], int count)
        : uname(n), uid(id), mail(m), p_size(count) {
        h_pwd = hashPassword(pwd);
        for (int i = 0; i < count; i++) perms[i] = p[i];
    }

    virtual void show() {
        cout << uname << " | ID: " << uid << " | Email: " << mail << endl;
    }

    bool checkPass(string pwd) {
        return h_pwd == hashPassword(pwd);
    }

    virtual bool labAccess() {
        for (int i = 0; i < p_size; i++)
            if (perms[i] == "lab_control") return true;
        return false;
    }

    friend void performAction(Member* u, string act);
};

void performAction(Member* u, string act) {
    for (int i = 0; i < u->p_size; i++) {
        if (u->perms[i] == act) {
            cout << "Access granted: " << act << endl;
            return;
        }
    }
    cout << "Access denied: " << act << endl;
}

class Learner : public Member {
protected:
    int tasks[10];
public:
    Learner(string n, int id, string m, string pwd)
        : Member(n, id, m, pwd, new string[1]{"submit"}, 1) {
        for (int i = 0; i < 10; i++) tasks[i] = 0;
    }

    void show() override {
        cout << "[Student] ";
        Member::show();
    }

    void assign(int idx) {
        if (idx >= 0 && idx < 10) tasks[idx] = 0;
    }

    void markDone(int idx) {
        if (idx >= 0 && idx < 10) tasks[idx] = 1;
    }
};

class Assistant : public Learner {
    Learner* stu_list[10];
    string proj[2];
    int sc, pc;
public:
    Assistant(string n, int id, string m, string pwd)
        : Learner(n, id, m, pwd), sc(0), pc(0) {
        perms[0] = "projects";
        perms[1] = "manage";
        p_size = 2;
    }

    void show() override {
        cout << "[TA] ";
        Member::show();
    }

    void linkStudent(Learner* s) {
        if (sc < 10) stu_list[sc++] = s;
    }

    void addProj(string pr) {
        if (pc < 2) proj[pc++] = pr;
    }

    void viewProj() {
        for (int i = 0; i < pc; i++) cout << "- " << proj[i] << endl;
    }
};

class Supervisor : public Member {
public:
    Supervisor(string n, int id, string m, string pwd)
        : Member(n, id, m, pwd, new string[2]{"projects", "lab_control"}, 2) {}

    void show() override {
        cout << "[Professor] ";
        Member::show();
    }

    void assignWork(Assistant& a, string topic) {
        a.addProj(topic);
    }
};

int main() {
    Learner stud("Anwer", 111, "k240033@nu.edu", "soDissapointInFast");
    Assistant ta("Baqar", 232, "k240006@nu.edu", "dimaagKharabFastKa");
    Supervisor prof("Dr. Rafi", 555, "muhammadrafi12345678@gmail.com", "iAmPRO");

    stud.show();
    cout << (stud.checkPass("soDissapointInFast") ? "Login success\n" : "Login failed\n");
    stud.assign(1);
    stud.markDone(1);

    ta.show();
    ta.linkStudent(&stud);
    ta.addProj("Research AI");
    ta.viewProj();

    prof.show();
    prof.assignWork(ta, "Quantum Comp");

    performAction(&stud, "submit");
    performAction(&ta, "manage");
    performAction(&prof, "lab_control");

    return 0;
}
