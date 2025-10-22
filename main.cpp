#include "code.h"

int main()
{


    Team chiefs = {"Chiefs", {}};
    Player brown = {"Brown", PlayerPosition::WR, "chiefs", 5, 81, {}};
    chiefs.roster = {
        {"Mahomes", PlayerPosition::QB, "chiefs", 15, 99, {}},
        {"Kelce", PlayerPosition::TE, "chiefs", 87, 99, {}},
        {"Pacheco", PlayerPosition::RB, "chiefs", 10, 87, {}},
        brown,
        {"Rice", PlayerPosition::WR, "chiefs", 4, 81, {}},
        {"Edwards-Helaire", PlayerPosition::RB, "chiefs", 25, 75, {}}
    };
    
    Scoreboard scoreboard;
    
    
    simulatePossession(chiefs, scoreboard);
    displayScoreboard(chiefs.name, "Patriots", scoreboard);
    displayTeamStats(chiefs);
    

    return 0;
}

