#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Ball 
{
public:
    int x, y;
    Ball() : x(0), y(0) {}
    void move(int dx, int dy) 
    { 
        x += dx; 
        y += dy; 
    }
    pair<int, int> getPosition() 
    {
        return {x, y}; 
    }
};

class Goal 
{
public:
    const int x = 3, y = 3;
    bool goal_reach(int ball_X, int ball_Y) 
    { 
        return ball_X == x && ball_Y == y; 
    }
};

class Robot 
{
public:
    string name;
    int hits;
    Robot(string n) : name(n), hits(0) {}
    void hit_ball(Ball &ball, string direction) 
    {
        hits++;
        if (direction == "up") 
        {
            ball.move(0, 1);
        }
        else if (direction == "down") 
        {
            ball.move(0, -1);
        }
        else if (direction == "left") 
        {
            ball.move(-1, 0);
        }
        else if (direction == "right") 
        {
            ball.move(1, 0);
        }    
    }
};

class Team 
{
public:
    string team_name;
    Robot* player;
    Team(string name, string r_name) : team_name(name) 
    {
         player = new Robot(r_name); 
    }
    
    ~Team() 
    {
        delete player;
    }
};

class Game 
{
public:
    Team* team_1;
    Team* team_2;
    Ball ball;
    Goal goal;

    Game(string team1_name, string robot1_name, string team2_name, string robot2_name) 
    {
        team_1 = new Team(team1_name, robot1_name);
        team_2 = new Team(team2_name, robot2_name);
    }

    ~Game() 
    {
        delete team_1; 
        delete team_2;
    }

    void play(Team* team) 
    {
        ball = Ball();
        while (!goal.goal_reach(ball.x, ball.y)) 
        {
            string commands[] = {"up", "down", "left", "right"};
            string movement = commands[rand() % 4];
            team->player->hit_ball(ball, movement);
        }
    }

    void startGame() 
    {
        srand(static_cast<unsigned int>(time(0)));
        play(team_1);
        int hits_1 = team_1->player->hits;
        play(team_2);
        int hits_2 = team_2->player->hits;
        winner(hits_1, hits_2);
    }

    void winner(int hits_1, int hits_2) 
    {
        if (hits_1 < hits_2) 
        {
            cout << team_1->team_name << " wins with " << hits_1 << " hits" << endl;
        }
        else if (hits_2 < hits_1) 
        {
            cout << team_2->team_name << " wins with " << hits_2 << " hits" << endl;
        }
        else 
        {
            cout << "It's a tie!" << endl;
        }
    }
};

int main() 
{
    Game game("Dragon", "Robot A", "Devil", "Robot B");
    game.startGame();
    return 0;
}
