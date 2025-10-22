#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

enum class PlayerPosition
{
    QB,
    RB,
    WR,
    TE,
    NUM_POSITIONS
};

enum class Play
{
    PASSING,
    RUSHING,
    NUM_PLAYS
};

struct PlayerStats
{
    int num_pass_attempts = 0;
    int num_pass_completions = 0;
    int num_rush_attempts = 0;
    int num_touchdowns = 0;
    int yards_passing = 0;
    int yards_rushing = 0;
};

struct Player
{
    std::string name = "missing";
    PlayerPosition position = PlayerPosition::NUM_POSITIONS;
    std::string team_name = "missing";
    int jersey_number = -1;
    int rating = 0;
    PlayerStats game_stats = {};
};

struct Team
{
    std::string name = "missing";
    std::vector<Player> roster = {};
};

struct Scoreboard
{
    int home_score = 0;
    int away_score = 0;
    int current_down = 0;
    int yards_to_go = 0;
};

bool observeBernoulli(double p, int seed);
int observeUniformInt(int min, int max, int seed);
double calcPlayProb(const Team& team);
Player& getQB(Team& team);
Player& getPlayer(Team& team, Play play, int seed);
bool execPassingPlay(Player& qb, Player& receiver, Scoreboard& scoreboard, int seed);
bool execRushingPlay(Player& qb, Player& back, Scoreboard& scoreboard, int seed);
void displayScoreboard(const string& home_name, const string& away_name, const Scoreboard& scoreboard);
void displayTeamStats(const Team& team);
void simulatePossession(Team& team, Scoreboard& scoreboard);