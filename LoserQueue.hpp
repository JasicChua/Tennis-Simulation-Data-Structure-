#ifndef LOSERQUEUE_HPP
#define LOSERQUEUE_HPP
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Node
struct LoserNode {
    string playerID;
    string playerName;
    int playerScore;
    LoserNode* nextNode;
    // Constructor
    LoserNode(string id, string name, int score);
};

// Singleton Class
class LoserQueue {
private:
    static LoserQueue* loserQueue; // Singleton instance
    LoserNode* front;
    int size;
    // Private constructor
    LoserQueue();


public:
    // Get instance 
    static LoserQueue* getInstance();

    // Delete instance
    static void deleteInstance();

    // Destructor
    ~LoserQueue();

    // Enqueue
    void enqueue(string playerID, string playerName, int playerScore);

    // Remove and return the highest score loser
    LoserNode* dequeueHighest();

    // Peek highest score without removing
    //LoserNode* peekHighest();

    // Get the current size of the queue
    int getSize();

    // Check if queue is empty
    bool isEmpty();

    // Print all players in the queue
    void displayPlayers();

    bool isPlayerInQueue(string playerID);

    
};

#endif
