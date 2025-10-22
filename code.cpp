#include "code.h"
#include <iostream>
#include <random>
#include <chrono>
#include <stdexcept>
using namespace std;

bool observeBernoulli(double p, int seed) 
{
    if (p < 0.0 || p > 1.0) throw runtime_error("Negative value not allowed");
    mt19937 gen(seed);
    bernoulli_distribution dist(p);
    return dist(gen);
}

int observeUniformInt(int min, int max, int seed) 
{
    if (min > max) throw runtime_error("Negative value not allowed");
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

double calcPlayProb(const Team& team) 
{
    double pass_rating = 0.0, rush_rating = 0.0;
    int pass_count = 0, rush_count = 0;
    
    for (const auto& player : team.roster) 
    {
        if (player.position == PlayerPosition::WR || player.position == PlayerPosition::TE) 
        {
            pass_rating += player.rating;
            pass_count++;
        }
        if (player.position == PlayerPosition::RB || player.position == PlayerPosition::QB || player.position == PlayerPosition::TE) 
        {
            rush_rating += player.rating;
            rush_count++;
        }
    }
    
    pass_rating = (pass_count > 0) ? pass_rating / pass_count : 0.0;
    rush_rating = (rush_count > 0) ? rush_rating / rush_count : 0.0;
    
    return (pass_rating + rush_rating == 0.0) ? 0.5 : pass_rating / (pass_rating + rush_rating);
}

Player& getQB(Team& team) 
{
    for (auto& player : team.roster) 
    {
        if (player.position == PlayerPosition::QB) return player;
    }
    throw runtime_error("No quarterback found");
}

Player& getPlayer(Team& team, Play play, int seed) 
{
    vector<int> indices;
    for (size_t i = 0; i < team.roster.size(); i++) 
    {
        if ((play == Play::PASSING && (team.roster[i].position == PlayerPosition::WR || team.roster[i].position == PlayerPosition::TE)) ||
            (play == Play::RUSHING && (team.roster[i].position == PlayerPosition::RB || team.roster[i].position == PlayerPosition::QB || team.roster[i].position == PlayerPosition::TE))) 
        {
            indices.push_back(static_cast<int>(i));
        }
    }
    if (indices.empty()) throw runtime_error("No eligible players");
    
    int max_index = static_cast<int>(indices.size()) - 1;
    int random_index = observeUniformInt(0, max_index, seed);
    return team.roster[indices[random_index]];
}

bool execPassingPlay(Player& qb, Player& receiver, Scoreboard& sb, int seed) 
{
    qb.game_stats.num_pass_attempts++;
    receiver.game_stats.num_pass_attempts++;
    
    bool success = observeBernoulli((qb.rating + receiver.rating) / 300.0, seed);
    
    if (success) 
    {
        qb.game_stats.num_pass_completions++;
        receiver.game_stats.num_pass_completions++;
        
        int yards = observeUniformInt(1, 10, seed);
        qb.game_stats.yards_passing += yards;
        receiver.game_stats.yards_passing += yards;
        
        sb.yards_to_go -= yards;
        
        if (sb.yards_to_go <= 0) 
        {
            sb.home_score += 6;
            qb.game_stats.num_touchdowns++;
            receiver.game_stats.num_touchdowns++;
            sb.yards_to_go = 0;
        } 
        else 
        {
            sb.current_down++;
        }
    } 
    else 
    {
        sb.current_down++;
    }
    
    return success;
}

bool execRushingPlay(Player& qb, Player& rusher, Scoreboard& sb, int seed) 
{
    rusher.game_stats.num_rush_attempts++;
    
    bool success = observeBernoulli((qb.rating + rusher.rating) / 300.0, seed);
    
    if (success) 
    {
        int yards = observeUniformInt(1, 5, seed);
        rusher.game_stats.yards_rushing += yards;
        
        sb.yards_to_go -= yards;
        
        if (sb.yards_to_go <= 0) 
        {
            sb.home_score += 6;
            rusher.game_stats.num_touchdowns++;
            sb.yards_to_go = 0;
        } else {
            sb.current_down++;
        }
    } else {
        sb.current_down++;
    }
    
    return success;
}

void displayScoreboard(const string& home_name, const string& away_name, const Scoreboard& sb) 
{
    cout << "Final score:\n" << home_name << ": " << sb.home_score << " - " << away_name << ": " << sb.away_score << "\n\n";
}

void displayTeamStats(const Team& team) 
{
    cout << "Game statistics of " << team.name << " players:\n";
    for (const auto& player : team.roster) 
    {
        cout << player.name << " contributed to " << player.game_stats.num_touchdowns << " touchtown(s). ";
        
        if (player.position == PlayerPosition::QB) 
        {
            cout << "He threw " << player.game_stats.num_pass_completions << " successful pass(es) out of "
                 << player.game_stats.num_pass_attempts << " attempt(s) with " << player.game_stats.yards_passing
                 << " passed yard(s). He had " << player.game_stats.num_rush_attempts << " rush attempt(s) with "
                 << player.game_stats.yards_rushing << " rushed yard(s).";
        } 
        else if (player.position == PlayerPosition::WR || player.position == PlayerPosition::TE) 
        {
            cout << "He caught " << player.game_stats.num_pass_completions << " pass(es) out of "
                 << player.game_stats.num_pass_attempts << " attemp(s) with " << player.game_stats.yards_passing
                 << " passed yard(s). He had " << player.game_stats.num_rush_attempts << " rush attempt(s) with "
                 << player.game_stats.yards_rushing << " rushed yard(s).";
        } 
        else 
        {
            cout << "He had " << player.game_stats.num_rush_attempts << " rush attempt(s) with "
                 << player.game_stats.yards_rushing << " rushed yard(s).";
        }
        cout << "\n";
    }
}

void simulatePossession(Team& team, Scoreboard& sb) 
{
    int base_seed = static_cast<int>(chrono::steady_clock::now().time_since_epoch().count());
    
    auto wait = []() 
    {
        cout << "Press Enter to continue...\n\n";
        string dummy;
        getline(cin, dummy);
    };
    
    cout << "The " << team.name << " have the possession.\n";
    wait();
    
    sb.current_down = 1;
    sb.yards_to_go = 10;
    int distance_to_goal = 10;
    
    while (sb.current_down <= 4 && distance_to_goal > 0) 
    {
        string downs[] = {"First", "Second", "Third", "Fourth"};
        cout << downs[sb.current_down - 1] << " and " << sb.yards_to_go << "\n";
        wait();
        
        bool is_passing = observeBernoulli(calcPlayProb(team), base_seed + sb.current_down);
        
        if (is_passing) {
            cout << "Passing play is called.\n";
            wait();
            
            Player& qb = getQB(team);
            Player& receiver = getPlayer(team, Play::PASSING, base_seed + sb.current_down + 100);
            bool success = execPassingPlay(qb, receiver, sb, base_seed + sb.current_down + 200);
            
            cout << "Passing play is " << (success ? "successful" : "NOT successful") << ".\n";
            wait();
            
            if (success && sb.yards_to_go <= 0) 
            {
                cout << "Touchdown scored.\nPossession is over...\n";
                wait();
                break;
            }
        } 
        else 
        {
            cout << "Rushing play is called.\n";
            wait();
            
            Player& qb = getQB(team);
            Player& rusher = getPlayer(team, Play::RUSHING, base_seed + sb.current_down + 300);
            bool success = execRushingPlay(qb, rusher, sb, base_seed + sb.current_down + 400);
            
            cout << "Rushing play is " << (success ? "successful" : "NOT successful") << ".\n";
            wait();
            
            if (success && sb.yards_to_go <= 0) 
            {
                cout << "Touchdown scored.\nPossession is over.\n";
                wait();
                break;
            }
        }
        
        if (sb.current_down > 4) 
        {
            cout << "No more downs, possession is turned over to the other team.\nPossession is over...\n";
        }
    }
}