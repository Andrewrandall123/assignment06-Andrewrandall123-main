#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "code.h"
#include <iostream>
#include <sstream>



TEST_CASE("Test01: PlayerPosition enum class", "[3 points]") 
{
    {
    INFO("Check if PlayerPosition is an enum");
        STATIC_REQUIRE(std::is_enum<PlayerPosition>::value);
    }    
    {
    INFO("Check if PlayerPosition is an enum class and not a plain enum");
        STATIC_REQUIRE(!std::is_convertible<PlayerPosition, int>::value);
    }
    INFO("Check if PlayerPosition enum has 5 elements.");
        CHECK(static_cast<int>(PlayerPosition::NUM_POSITIONS) == 4);
}

TEST_CASE("Test02: Play enum class", "[2 points]") 
{
    {
    INFO("Check if Play is an enum");
        STATIC_REQUIRE(std::is_enum<Play>::value);
    }    
    {
    INFO("Check if Play is an enum class and not a plain enum");
        STATIC_REQUIRE(!std::is_convertible<Play, int>::value);
    }
    INFO("Check if Play enum has 3 elements.");
        CHECK(static_cast<int>(Play::NUM_PLAYS) == 2);
}

TEST_CASE("Test03: PlayerStats struct", "[3 points]") {
    {
    INFO("Check if PlayerStats is a class/struct type");
        STATIC_REQUIRE(std::is_class<PlayerStats>::value);
    }
    {
    INFO("Check if PlayerStats has correct element names and default initialization");
        PlayerStats stats;
        CHECK(stats.num_pass_attempts == 0);
        CHECK(stats.num_pass_completions == 0);
        CHECK(stats.num_rush_attempts == 0);
        CHECK(stats.num_touchdowns == 0);
        CHECK(stats.yards_passing == 0);
        CHECK(stats.yards_rushing == 0);
    }
    {
    INFO("Check the number of elements in PlayerStats");
        struct TestPlayerStats {
            int num_pass_attempts = 0;
            int num_pass_completions = 0;
            int num_rush_attempts = 0;
            int num_touchdowns = 0;
            int yards_passing = 0;
            int yards_rushing = 0;
        } test_stats;
        PlayerStats stats;
        CHECK(sizeof(test_stats) == sizeof(stats));
    }
}

TEST_CASE("Test04: Player struct", "[3 points]") {
    {
    INFO("Check if Player is a class/struct type");
        STATIC_REQUIRE(std::is_class<Player>::value);
    }
    {
    INFO("Check if Player has correct element names and default initialization");
        Player player;
        PlayerStats stats;
        CHECK(player.name == "missing");
        CHECK(player.position == PlayerPosition::NUM_POSITIONS);
        CHECK(player.team_name == "missing");
        CHECK(player.jersey_number == -1);
        CHECK(player.rating == 0);
        CHECK(player.game_stats.num_pass_attempts == 0);
    }
    {
    INFO("Check the number of elements in Player");
        struct Player {
            std::string name = "missing";
            PlayerPosition position = PlayerPosition::NUM_POSITIONS;
            std::string team_name = "missing";
            int jersey_number = -1;                      
            int rating = 0;
            PlayerStats game_stats {};
        } test_player;
        Player player;
        CHECK(sizeof(test_player) == sizeof(player));
    }
}

TEST_CASE("Test05: Team struct", "[2 points]") {
    {
    INFO("Check if Team is a class/struct type");
        STATIC_REQUIRE(std::is_class<Team>::value);
    }
    {
    INFO("Check if Team has correct element names and default initialization");
        Team team;
        CHECK(team.name == "missing");
        CHECK(team.roster.empty());
    }
    {
    INFO("Check the number of elements in Team");
        struct Team {
            std::string name = "missing";
            std::vector<Player> roster;
        } test_team;
        Team team;
        CHECK(sizeof(test_team) == sizeof(team));
    }
}

TEST_CASE("Test06: Scoreboard struct", "[2 points]") {
    {
    INFO("Check if Scoreboard is a class/struct type");
        STATIC_REQUIRE(std::is_class<Scoreboard>::value);
    }
    {
    INFO("Check if Scoreboard has correct element names and default initialization");
        Scoreboard scoreboard;
        CHECK(scoreboard.home_score == 0);
        CHECK(scoreboard.away_score == 0);
        CHECK(scoreboard.current_down == 0);
        CHECK(scoreboard.yards_to_go == 0);
    }
    {
    INFO("Check the number of elements in Scoreboard");
        struct Scoreboard {
            int home_score = 0;
            int away_score = 0; 
            int current_down = 0;
            int yards_to_go = 0;
        } test_scoreboard;
        Scoreboard scoreboard;
        CHECK(sizeof(test_scoreboard) == sizeof(scoreboard));
    }
}

//

TEST_CASE("Test07: observeBernoulli() function with invalid parameter", "[2 points]") 
{
    {
    INFO("observeBernoulli() function with invalid parameter.");
        CHECK_THROWS_AS(observeBernoulli(1.01, rand()), std::runtime_error);
        CHECK_THROWS_AS(observeBernoulli(-0.01, rand()), std::runtime_error);
    }
}

TEST_CASE("Test08: observeBernoulli() function with p = 0 or p = 1", "[1 points]") 
{
    {
    INFO("observeBernoulli() function with p = 0. Always false.");
        double p = 0;
        int sample_size = 100;
        int true_cases = 0;
        for(int i = 0; i < sample_size; i++) {
            if (observeBernoulli(p, rand())) ++true_cases;
        }
        CHECK(true_cases == 0);
    }
    {
    INFO("observeBernoulli() function with p = 1. Always true.");
        double p = 1;
        int sample_size = 100;
        int true_cases = 0;
        for(int i = 0; i < sample_size; i++) {
            if (observeBernoulli(p, rand())) ++true_cases;
        }
        CHECK(true_cases == sample_size);
    }
}

TEST_CASE("Test09: observeBernoulli() function with 0 < p < 1", "[2 points]") 
{
    {
    INFO("observeBernoulli() function with p = 0.25.");
        double p = 0.25;
        int sample_size = 10'000;
        int true_cases = 0;
        for(int i = 0; i < sample_size; i++) {
            if (observeBernoulli(p, rand())) ++true_cases;
        }
        CHECK_THAT(true_cases/double(sample_size), Catch::Matchers::WithinAbs(0.25, 0.1));
    }
    {
    INFO("observeBernoulli() function with p = 0.5.");
        double p = 0.5;
        int sample_size = 10'000;
        int true_cases = 0;
        for(int i = 0; i < sample_size; i++) {
            if (observeBernoulli(p, rand())) ++true_cases;
        }
        CHECK_THAT(true_cases/double(sample_size), Catch::Matchers::WithinAbs(0.5, 0.1));
    }
    {
    INFO("observeBernoulli() function with p = 0.75.");
        double p = 0.75;
        int sample_size = 10'000;
        int true_cases = 0;
        for(int i = 0; i < sample_size; i++) {
            if (observeBernoulli(p, rand())) ++true_cases;
        }
        CHECK_THAT(true_cases/double(sample_size), Catch::Matchers::WithinAbs(0.75, 0.1));
    }
}

TEST_CASE("Test10: observeBernoulli() function with a known seed", "[2 points]") 
{
    {
    INFO("observeBernoulli() function with seed = 0.");
        double p = 0.25;
        int seed = 0;
        int sample_size = 100;
        int true_cases = 0;
        for(int i = 0; i < sample_size; i++) {
            if (observeBernoulli(p, seed)) ++true_cases;
        }
        CHECK(true_cases == 0);
    }
    {
    INFO("observeBernoulli() function with seed = 3.");
        double p = 0.25;
        int seed = 3;
        int sample_size = 100;
        int true_cases = 0;
        for(int i = 0; i < sample_size; i++) {
            if (observeBernoulli(p, seed)) ++true_cases;
        }
        CHECK(true_cases == sample_size);
    }
}



// 

TEST_CASE("Test11: observeUniformInt() function with invalid range", "[2 points]") 
{
    {
    INFO("observeUniformInt() function with invalid range.");
        CHECK_THROWS_AS(observeUniformInt(6, 5, rand()), std::runtime_error);
        CHECK_THROWS_AS(observeUniformInt(-5, -6, rand()), std::runtime_error);
    }
}

TEST_CASE("Test12: observeUniformInt() function with min=max", "[2 points]") 
{
    {
    INFO("observeUniformInt() function with min=max.");
        int min = 5;
        int max = 5;
        int sample_size = 100;
        double sum = 0;
        for(int i = 0; i < sample_size; i++) {
            sum += observeUniformInt(min, max, rand());
        }
        REQUIRE(sum/sample_size == 5);
    }
}

TEST_CASE("Test13: observeUniformInt() function with min < max", "[2 points]") 
{
    {
    INFO("observeUniformInt() function with min = 1, max = 6.");
        int min = 1;
        int max = 6;
        int sample_size = 10'000;
        double sum = 0;
        for(int i = 0; i < sample_size; i++) {
            sum += observeUniformInt(min, max, rand());
        }
        CHECK_THAT(sum/sample_size, Catch::Matchers::WithinAbs(3.5, 0.1));
    }
    {
    INFO("observeUniformInt() function with min = 0, max = 3.");
        int min = 0;
        int max = 3;
        int sample_size = 10'000;
        double sum = 0;
        for(int i = 0; i < sample_size; i++) {
            sum += observeUniformInt(min, max, rand());
        }
        CHECK_THAT(sum/sample_size, Catch::Matchers::WithinAbs(1.5, 0.1));
    }
    {
    INFO("observeUniformInt() function with min = -3, max = 3.");
        int min = -3;
        int max = 3;
        int sample_size = 10'000;
        double sum = 0;
        for(int i = 0; i < sample_size; i++) {
            sum += observeUniformInt(min, max, rand());
        }
        CHECK_THAT(sum/sample_size, Catch::Matchers::WithinAbs(0, 0.1));
    }
}

TEST_CASE("Test14: observeUniformInt() function with a known seed", "[2 points]") 
{
    {
    INFO("observeUniformInt() function with seed = 0.");
        int min = 1;
        int max = 6;
        int seed = 0;
        int sample_size = 100;
        double sum = 0;
        for(int i = 0; i < sample_size; i++) {
            sum += observeUniformInt(min, max, seed);
        }
        CHECK_THAT(sum/sample_size, Catch::Matchers::WithinAbs(4.0, 0.001));
    }
    {
    INFO("observeUniformInt() function with seed = 1.");
        int min = 1;
        int max = 6;
        int seed = 1;
        int sample_size = 100;
        double sum = 0;
        for(int i = 0; i < sample_size; i++) {
            sum += observeUniformInt(min, max, seed);
        }
        CHECK_THAT(sum/sample_size, Catch::Matchers::WithinAbs(3.0, 0.001));
    }
    {
    INFO("observeUniformInt() function with seed = 4.");
        int min = 1;
        int max = 6;
        int seed = 4;
        int sample_size = 100;
        double sum = 0;
        for(int i = 0; i < sample_size; i++) {
            sum += observeUniformInt(min, max, seed);
        }
        CHECK_THAT(sum/sample_size, Catch::Matchers::WithinAbs(6.0, 0.001));
    }
}

//

TEST_CASE("Test15: calcPlayProb() function", "[5 points]") 
{
    {
    INFO("calcPlayProb() function for the Chiefs");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players};
        CHECK_THAT(calcPlayProb(chiefs), Catch::Matchers::WithinAbs(0.49, 0.01));
    }
    {
    INFO("calcPlayProb() function for a test team");
        std::vector<Player> players = {
            {"Player 1", PlayerPosition::QB, "Test team", 1, 90},
            {"Player 2", PlayerPosition::WR, "Test team", 2, 50},
            {"Player 3", PlayerPosition::RB, "Test team", 3, 90},
            {"Player 4", PlayerPosition::RB, "Test team", 4, 90},
            {"Player 5", PlayerPosition::TE, "Test team", 5, 50},
            {"Player 6", PlayerPosition::WR, "Test team", 6, 50}
        };
        Team test_team {"Test team", players};
        CHECK_THAT(calcPlayProb(test_team), Catch::Matchers::WithinAbs(0.38, 0.01)); 
    }
}

// 

TEST_CASE("Test16: getQB() function", "[5 points]") 
{
    {
    INFO("getQB() function with a single QB in the team");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 

        Player& QB = getQB(chiefs);
        CHECK(QB.name == "Mahomes");
        CHECK(QB.position == PlayerPosition::QB);
        CHECK(QB.jersey_number == 15);
    }
}

//

TEST_CASE("Test17: getPlayer() function - passing play", "[5 points]") 
{   
    {
    INFO("getPlayer() function - passing play");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        int wr_count = 0;
        int te_count = 0;
        int sample_size = 1000;
        for (int i = 0; i < sample_size; ++i) {
            Player& receiver = getPlayer(chiefs, Play::PASSING, rand());
            if (receiver.position == PlayerPosition::WR) ++wr_count;
            if (receiver.position == PlayerPosition::TE) ++te_count;
        }
        
        CHECK(wr_count+te_count == sample_size);
        CHECK_THAT(wr_count/double(sample_size), Catch::Matchers::WithinAbs(0.66, 0.1));
        CHECK_THAT(te_count/double(sample_size), Catch::Matchers::WithinAbs(0.33, 0.1));
    }
}

TEST_CASE("Test18: getPlayer() function - rushing play", "[5 points]") 
{   
    {
    INFO("getPlayer() function - rushing play");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        int qb_count = 0;
        int rb_count = 0;
        int te_count = 0;
        int sample_size = 1000;
        for (int i = 0; i < sample_size; ++i) {
            Player& receiver = getPlayer(chiefs, Play::RUSHING, rand());
            if (receiver.position == PlayerPosition::QB) ++qb_count;
            if (receiver.position == PlayerPosition::RB) ++rb_count;
            if (receiver.position == PlayerPosition::TE) ++te_count;
        }
        
        CHECK(qb_count+rb_count+te_count == sample_size);
        CHECK_THAT(qb_count/double(sample_size), Catch::Matchers::WithinAbs(0.25, 0.1));
        CHECK_THAT(rb_count/double(sample_size), Catch::Matchers::WithinAbs(0.5, 0.1));
        CHECK_THAT(te_count/double(sample_size), Catch::Matchers::WithinAbs(0.25, 0.1));
    }
}

//

TEST_CASE("Test19: execPassingPlay() function - successful passing play, no touchdown", "[6 points]") 
{   
    {
    INFO("execPassingPlay() function - successful passing play, no touchdown");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 10;
        scoreboard.current_down = 1;
        Player& qb = chiefs.roster[0];
        Player& receiver = chiefs.roster[1];
        int seed = 0;

        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execPassingPlay(qb, receiver, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);

        CHECK(qb.game_stats.num_pass_attempts == 1);
        CHECK(receiver.game_stats.num_pass_attempts == 1);
        CHECK(isSuccess);
        CHECK(qb.game_stats.num_pass_completions == 1);
        CHECK(receiver.game_stats.num_pass_completions == 1);
        CHECK(scoreboard.yards_to_go == 4);
        CHECK(scoreboard.home_score == 0);
        CHECK(qb.game_stats.yards_passing == 6);
        CHECK(receiver.game_stats.yards_passing == 6);
    }
}

TEST_CASE("Test20: execPassingPlay() function - unsuccessful passing play", "[6 points]") 
{   
    {
    INFO("execPassingPlay() function - unsuccessful passing play");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 10;
        scoreboard.current_down = 1;
        Player& qb = chiefs.roster[0];
        Player& receiver = chiefs.roster[1];
        int seed = 1;
        
        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execPassingPlay(qb, receiver, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);
        
        CHECK(qb.game_stats.num_pass_attempts == 1);
        CHECK(receiver.game_stats.num_pass_attempts == 1);
        CHECK(!isSuccess);
        CHECK(qb.game_stats.num_pass_completions == 0);
        CHECK(receiver.game_stats.num_pass_completions == 0);
        CHECK(scoreboard.yards_to_go == 10);
        CHECK(scoreboard.home_score == 0);
        CHECK(qb.game_stats.yards_passing == 0);
        CHECK(receiver.game_stats.yards_passing == 0);
    }
}

TEST_CASE("Test21: execPassingPlay() function - successful passing play, with touchdown", "[6 points]") 
{   
    {
    INFO("execPassingPlay() function - successful passing play, with touchdown");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 10;
        scoreboard.current_down = 1;
        Player& qb = chiefs.roster[0];
        Player& receiver = chiefs.roster[1];
        int seed = 24;
        
        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execPassingPlay(qb, receiver, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);
        
        CHECK(qb.game_stats.num_pass_attempts == 1);
        CHECK(receiver.game_stats.num_pass_attempts == 1);
        CHECK(isSuccess);
        CHECK(qb.game_stats.num_pass_completions == 1);
        CHECK(receiver.game_stats.num_pass_completions == 1);
        CHECK(scoreboard.yards_to_go == 0);
        CHECK(scoreboard.home_score == 6);
        CHECK(qb.game_stats.yards_passing == 10);
        CHECK(qb.game_stats.num_touchdowns == 1);
        CHECK(receiver.game_stats.num_touchdowns == 1);
        CHECK(receiver.game_stats.yards_passing == 10);
    }
}

//

TEST_CASE("Test22: execRushingPlay() function - successful rushing play, no touchdown", "[6 points]") 
{   
    {
    INFO("execRushingPlay() function - successful rushing play, no touchdown");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 5;
        scoreboard.current_down = 1;
        Player& qb = chiefs.roster[0];
        Player& back = chiefs.roster[2];
        int seed = 0;
        
        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execRushingPlay(qb, back, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);
        
        CHECK(qb.game_stats.num_rush_attempts == 0);
        CHECK(back.game_stats.num_rush_attempts == 1);
        CHECK(isSuccess);
        CHECK(scoreboard.yards_to_go == 2);
        CHECK(scoreboard.home_score == 0);
        CHECK(qb.game_stats.yards_rushing == 0);
        CHECK(back.game_stats.yards_rushing == 3);
    }
}

TEST_CASE("Test23: execRushingPlay() function - successful rushing play, no touchdown, QB rushing", "[6 points]") 
{   
    {
    INFO("execRushingPlay() function - successful rushing play, no touchdown, QB rushing");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 5;
        scoreboard.current_down = 1;
        Player& qb =  chiefs.roster[0];
        Player& back = chiefs.roster[0];
        int seed = 0;
        
        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execRushingPlay(qb, back, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);

        CHECK(qb.game_stats.num_rush_attempts == 1);
        CHECK(back.game_stats.num_rush_attempts == 1);
        CHECK(isSuccess);
        CHECK(scoreboard.yards_to_go == 2);
        CHECK(scoreboard.home_score == 0);
        CHECK(qb.game_stats.yards_rushing == 3);
        CHECK(back.game_stats.yards_rushing == 3);
    }
}

TEST_CASE("Test24: execRushingPlay() function - unsuccessful rushing play", "[6 points]") 
{   
    {
    INFO("execRushingPlay() function - unsuccessful rushing play");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 5;
        scoreboard.current_down = 1;
        Player& qb = chiefs.roster[0];
        Player& back = chiefs.roster[2];
        int seed = 1;
        
        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execRushingPlay(qb, back, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);

        CHECK(qb.game_stats.num_rush_attempts == 0);
        CHECK(back.game_stats.num_rush_attempts == 1);
        CHECK(!isSuccess);
        CHECK(scoreboard.yards_to_go == 5);
        CHECK(scoreboard.home_score == 0);
        CHECK(qb.game_stats.yards_rushing == 0);
        CHECK(back.game_stats.yards_rushing == 0);
    }
    {
    INFO("execRushingPlay() function - unsuccessful rushing play, QB rushing");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 5;
        scoreboard.current_down = 1;
        Player& qb = chiefs.roster[0];
        Player& back = chiefs.roster[0];
        int seed = 1;
        // Redirect cin to a std::stringstream filled with newlines
        
        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execRushingPlay(qb, back, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);

        CHECK(qb.game_stats.num_rush_attempts == 1);
        CHECK(back.game_stats.num_rush_attempts == 1);
        CHECK(!isSuccess);
        CHECK(scoreboard.yards_to_go == 5);
        CHECK(scoreboard.home_score == 0);
        CHECK(qb.game_stats.yards_rushing == 0);
        CHECK(back.game_stats.yards_rushing == 0);
    }
}

TEST_CASE("Test25: execRushingPlay() function - successful rushing play, with touchdown", "[6 points]") 
{   
    {
    INFO("execRushingPlay() function - successful rushing play, with touchdown");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 5;
        scoreboard.current_down = 1;
        Player& qb = chiefs.roster[0];
        Player& back = chiefs.roster[2];
        int seed = 24;
        
        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execRushingPlay(qb, back, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);

        CHECK(qb.game_stats.num_rush_attempts == 0);
        CHECK(back.game_stats.num_rush_attempts == 1);
        CHECK(isSuccess);
        CHECK(scoreboard.yards_to_go == 0);
        CHECK(scoreboard.home_score == 6);
        CHECK(qb.game_stats.num_touchdowns == 1);
        CHECK(back.game_stats.num_touchdowns == 1);
        CHECK(qb.game_stats.yards_rushing == 0);
        CHECK(back.game_stats.yards_rushing == 5);
    }  
    {
    INFO("execRushingPlay() function - successful rushing play, with touchdown, QB rushing");
        std::vector<Player> players = {
            {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
            {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
            {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
            {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
            {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
            {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
        };
        Team chiefs = {"Chiefs", players}; 
        Scoreboard scoreboard;
        scoreboard.yards_to_go = 5;
        scoreboard.current_down = 1;
        Player& qb =  chiefs.roster[0];
        Player& back = chiefs.roster[0];
        int seed = 24;
        
        // Redirect cout to our std::stringstream buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::stringstream out;
        std::cout.rdbuf(out.rdbuf());
        // Redirect cin to a std::stringstream filled with newlines
        std::streambuf* cinBuf = std::cin.rdbuf();
        std::stringstream input;
        input << std::string(100, '\n');
        std::cin.rdbuf(input.rdbuf());
        
        bool isSuccess = execRushingPlay(qb, back, scoreboard, seed);
        // Restore the original buffer so further cout calls are normal
        std::cout.rdbuf(coutBuf);

        CHECK(qb.game_stats.num_rush_attempts == 1);
        CHECK(back.game_stats.num_rush_attempts == 1);
        CHECK(isSuccess);
        CHECK(scoreboard.yards_to_go == 0);
        CHECK(scoreboard.home_score == 6);
        CHECK(qb.game_stats.num_touchdowns == 1);
        CHECK(back.game_stats.num_touchdowns == 1);
        CHECK(qb.game_stats.yards_rushing == 5);
        CHECK(back.game_stats.yards_rushing == 5);
    }
}

//

int count(std::string str, std::string target) {
    std::stringstream out(str);
    int count = 0;
    std::string word;
    while (out >> word) {
    if (word == target) {
        ++count;
    }
    }
    return count;    
}

TEST_CASE("Test26: simulatePossession() function", "[8 points]") 
{   
    std::vector<Player> players = {
        {"Mahomes",         PlayerPosition::QB, "Chiefs", 15, 99, {}},
        {"Kelce",           PlayerPosition::TE, "Chiefs", 87, 99, {}},
        {"Pacheco",         PlayerPosition::RB, "Chiefs", 10, 87, {}},
        {"Brown",           PlayerPosition::WR, "Chiefs", 5, 81, {}},
        {"Rice",            PlayerPosition::WR, "Chiefs", 4, 81, {}},
        {"Edwards-Helaire", PlayerPosition::RB, "Chiefs", 25, 75, {}}
    };
    
    
    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }

    {
    Team chiefs = {"Chiefs", players}; 
    Scoreboard scoreboard;
    INFO("Four plays must be called without a touchdown. Console output must include Chiefs, First, and Possession.");
    // Redirect cout to our std::stringstream buffer
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    // Redirect cin to a std::stringstream filled with newlines
    std::streambuf* cinBuf = std::cin.rdbuf();
    std::stringstream input;
    input << std::string(100, '\n');
    std::cin.rdbuf(input.rdbuf());
    
    simulatePossession(chiefs, scoreboard);
    std::string str = out.str();
    // Restore the original buffer so further cout calls are normal
    std::cout.rdbuf(coutBuf);

    
        int r_count = count(str, "Rushing");
        int p_count = count(str, "Passing");
        int t_count = count(str, "Touchdown");
        CHECK( (((r_count + p_count) == 8 && t_count == 0) || ((r_count + p_count) <= 8 && t_count == 1)) );
        CHECK(count(str, "Chiefs") == 1 );
        CHECK(count(str, "First") == 1 );
        CHECK(count(str, "Possession") == 1 );
    }
    
}
