#ifndef MATCH_HPP
#define MATCH_HPP

#include<string>
using namespace std;

struct Match {
    string matchId;
    string stage;
    string date;
    string player1ID;
    string player2ID;

    Match(string matchId, string stage, string date, string player1ID, string player2ID);

};

class MatchList {};



#endif //MATCH_HPP
