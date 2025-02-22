#include <iostream>
using namespace std;

class Skill {
public:

    int skill_id;
    string skill_name;
    string desc;

    Skill(int id, string name, string desc) : skill_id(id), skill_name(name), desc(desc) {}
    
    void show_skill_details() 

    { 
        cout << "SKILL ID: " << skill_id << "\nName: " << skill_name << ", Descrip: " << desc << endl;
    }
        
    void upd_skill_desc(string new_desc){
        desc = new_desc;
    }
};    

class Sport{   
public:
    int sport_id;

    string name;
    string desc;
    Skill req_skill;

    Sport(int id, string n, string desc, Skill s) : sport_id(id), name(n), desc(desc), req_skill(s) {}
    
    void  set_skill(Skill s) 
    {
        req_skill = s;
    
    }
};

class Mentor;

class Student{
public:
    int student_id;
    string name;

    int age;
    Sport sports_intereST;
    Mentor* mentor_ass;


    Student(int id, string n, int a, Sport s) : student_id(id), name(n), age(a), sports_intereST(s), mentor_ass(nullptr) {}
    
    void reg_for_mentorship(Mentor* m);

    void view_mentor_details ();

};

class Mentor{

public:
    int mentor_id;
    string name;
    Sport sports_expertise;

    int max_learners;

    Student* assigned_learners[3];
     int learner_count;


    Mentor(int id, string n, int maxL, Sport s) : mentor_id(id), name(n), max_learners(maxL), sports_expertise(s), learner_count(0) {}
    

    bool assign_learner(Student* s) 
    {
        if (learner_count < max_learners)
        
         {
            assigned_learners[learner_count++] = s;
            s->mentor_ass =this;
            return true;
        } else 
        
        {
            cout << "Mentor " << name << " limit has been reache." << endl;
            return false;
        }
    }
    
    void remove_learner(Student* s) 
    {
        for (int i = 0; i < learner_count; i++) 
        {
            if (assigned_learners[i] == s) 
            
            {
                for (int j = i; j < learner_count - 1; j++) 
                
                {
                    assigned_learners[j] = assigned_learners[j + 1];
                }

                learner_count--;
                s->mentor_ass = nullptr;   break;
            }
        }
    }//removefun
    
    void view_learners(){
        cout << "Mentor: " << name << " Learner:" << endl;
        for (int i= 0; i < learner_count; i++) 
        {

            cout << "-" << assigned_learners[i]->name << endl;
        }
    }//vieww
    
    void provide_guidance() 
    
    {
        cout << "Mentor " << name<<" is providing guidance." << endl;
    }
};

void Student::reg_for_mentorship(Mentor* m) {
    if (m->assign_learner(this)) {
        mentor_ass = m;
    }
}

void Student::view_mentor_details() {
    if (mentor_ass) {
        cout << "Mentor Assigned: " << mentor_ass->name << endl;
    } else {
        cout << "No mentor assigned." << endl;
    }
}

int main() {
    Skill tennisSkill(1,"Serve" , "Powerful serves in Tennis");
    Sport tennis(1,"Tennis","A racket sport",tennisSkill);
    Mentor ali(1 ,"Ali", 3, tennis);
    Student saad(101, "Saad", 20, tennis);

    saad.reg_for_mentorship (&ali);
    saad. view_mentor_details();

    ali.view_learners();

    ali.provide_guidance();
    
    return 0;
}
