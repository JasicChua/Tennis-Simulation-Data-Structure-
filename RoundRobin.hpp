#ifndef ROUNDROBIN_HPP
#define ROUNDROBIN_HPP

#include <string>
using namespace std;
#include "Qualifier.hpp"

struct RoundRobin {
    string matchID;
    string playerID;
    string playerName;
    int playerScore;
    RoundRobin *nextNode;

    RoundRobin(string playerID, string playerName);
};

class rrList{
    string listName;
    int size;
    RoundRobin *front;
    RoundRobin *rear;

public:
    rrList(string listName);
    ~rrList();

    void enqueue(string playerID, string playerName);
    RoundRobin* dequeue();
    void displayRoundRobin();
    void getSize();

    void scheduleRoundRobin(qualifierList &qList);

    void roundRobinStart(playerList* wList);
    void displayResult();

    void displayPlayers();

    RoundRobin* getFront();
    bool isPlayerWithdrawn(string& playerID, playerList* wList);
};

#endif