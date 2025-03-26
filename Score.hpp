#pragma once
#ifndef SCORE_HPP
#define SCORE_HPP
#include "RoundRobin.hpp"
#include "Player.hpp"
#include "LoserQueue.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Global counters for persistent IDs across matches
// static int matchCount = 0;
static int setCount = 0;
static int gameCount = 0;
static int pointCount = 0;

// Enum for tennis score points.
enum PointScore {
    LOVE = 0,
    FIFTEEN = 1,
    THIRTY = 2,
    FORTY = 3,
    GAME = 4,
    ADVANTAGE = 5
};

struct ScoreNode {
    string ScoreID;     // Primary key e.g., "P001"
    string GameID;      // Foreign key e.g., "G001"
    int score1 = PointScore::LOVE, score2 = PointScore::LOVE;  // Raw point counts
    ScoreNode* next;

    ScoreNode(string scoreID, string gameID, int score1, int score2) {
        this->ScoreID = scoreID;
        this->GameID = gameID;
        this->score1 = score1;
        this->score2 = score2;
        this->next = nullptr;
    }
};

class ScoreQueue {
private:
    ScoreNode* front;
    ScoreNode* rear;
    int size = 0;
    string currentGameID;  // Track the current game ID
public:
    ScoreQueue();
    ~ScoreQueue();
    void enqueue(string scoreID, string gameID, int score1, int score2);
    ScoreNode* dequeue();
    void displayScore();
    bool isEmpty();

    void setcurrentGameID(string gameID);
    ScoreNode* getFront();
    ScoreNode* getRear();
    int getSize();
    string getCurrentGameID();
};

struct GameNode {
    string GameID;      // Primary key e.g., "G001"
    string SetID;       // Foreign key e.g., "S001"
    int p1Game, p2Game; // 1 indicates a win for that game.
    GameNode* next;
    GameNode(string gameID, string setID, int p1Game, int p2Game) {
        this->GameID = gameID;
        this->SetID = setID;
        this->p1Game = p1Game;
        this->p2Game = p2Game;
        this->next = nullptr;
    }
};

class GameQueue {
private:
    GameNode* front;
    GameNode* rear;
    int size = 0;
    string currentSetID;  // Track the current set ID
public:
    GameQueue();
    ~GameQueue();
    void enqueue(string gameID, string setID, int p1, int p2);
    // void enqueue(string gameID, int p1, int p2, ScoreQueue* scoreQueue);

    GameNode* dequeue();
    void displayGame();
    bool isEmpty();

    void setCurrentSetID(string setID);
    string getCurrentSetID();
    GameNode* getFront();
    GameNode* getRear();
    int getSize();
};

struct SetNode {
    string SetID;       // Primary key e.g., "S001"
    string MatchID;     // Foreign key e.g., "M001"
    int p1Set, p2Set;   // Total games won in that set.
    SetNode* next;
    SetNode(string setID, string matchID, int p1Set, int p2Set) {
        this->SetID = setID;
        this->MatchID = matchID;
        this->p1Set = p1Set;
        this->p2Set = p2Set;
        this->next = nullptr;
    }
};

class SetQueue {
private:
    SetNode* front;
    SetNode* rear;
    int size = 0;
    string currentMatchID;  // Track current match ID
public:
    SetQueue();
    ~SetQueue();
    void enqueue(string setID, string matchID, int p1Set, int p2Set);
    SetNode* dequeue();
    void displaySet();
    bool isEmpty();

    void setCurrentMatchID(string matchID);
    string getCurrentMatchID();
    SetNode* getFront();
    SetNode* getRear();
    int getSize();
};

struct MatchNode {
    string MatchID;
    Player* Player1;
    Player* Player2;
    MatchNode* next;
    MatchNode(string matchID, Player* player1, Player* player2) {
        this->MatchID = matchID;
        this->Player1 = player1;
        this->Player2 = player2;
        this->next = nullptr;
    }
};

class MatchList {
private:
    MatchNode* head;
    MatchNode* tail;
    int size = 0;
public:
    MatchList();
    ~MatchList();
    void insertEnd(string matchID, Player* player1, Player* player2);

    MatchNode* getHead();
    MatchNode* getTail();
    int getSize();
};

// Generate ID functions
string generateMatchID(int matchCount);
string generateSetID(int setCount);
string generateGameID(int gameCount);
string generatePointID(int pointCount);

string scoreToString(PointScore score);
string calculateScore(string matchID, string player1ID, string player1Name, string player2ID, string player2Name, playerList* wList);
string calculateScore(RoundRobin* player1, RoundRobin* player2, playerList* wList);

// File handling functions
void saveAllData(SetQueue& setQueue, GameQueue& gameQueue, ScoreQueue& scoreQueue);

void loadResultCSV(string& matchID, SetQueue& setQueue, GameQueue& gameQueue, ScoreQueue& scoreQueue);
void displaySpecificMatchHistory(string matchID, Player* player1, Player* player2);
void matchIDSelectionMenu(int validMatch, MatchList& matchList);
void displayPerformanceReport();

#endif //SCORE_HPP