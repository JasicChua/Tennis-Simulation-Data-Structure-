#ifndef KNOCKOUT_HPP
#define KNOCKOUT_HPP

#include <string>
using namespace std;
#include "RoundRobin.hpp"

struct Knockout {
    string matchID;
    string playerID;
    string playerName;
    Knockout *nextNode;

    Knockout(string playerID, string playerName);
};

class knockoutList {
    string listName;
    int size;
    Knockout *front;
    Knockout *rear;

public:
    knockoutList(string listName);
    ~knockoutList();

    void enqueue(string playerID, string playerName);
    Knockout *dequeue();
    void displayKnockout();
    int getSize();

    void scheduleSemi(rrList &rrListA, rrList &rrListB);
    void scheduleFinal();
    void knockoutStart(playerList* wList);
    void displayWinner();

    void displayPlayers();
    Knockout* getFront();
};


#endif //KNOCKOUT_HPP
