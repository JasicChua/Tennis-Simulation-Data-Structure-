
#ifndef QUALIFIERS_HPP
#define QUALIFIERS_HPP

#include <string>
using namespace std;
#include "Player.hpp"

struct Qualifier {
    string matchID;
    string playerID;
    string playerName;
    Qualifier *nextNode;

    Qualifier(string matchID, string playerID, string playerName);
};

class qualifierList {
    string listName;
    int size;
    Qualifier *front;
    Qualifier *rear;


public:
    qualifierList(string listName);
    ~qualifierList();

    void enqueue(string matchID, string player, string playerName);
    Qualifier* dequeue();
    void displayQualifier();
    int getSize();

    void scheduleQualifier(playerList &playerList);

    void qualifiersStart(playerList* wList);

    void displayResult();

    void displayPlayers();

    Qualifier* getFront();

};



#endif