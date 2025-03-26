#include "LoserQueue.hpp"


// Initialize static instance pointer
LoserQueue* LoserQueue::loserQueue = nullptr;

// Constructor
LoserNode::LoserNode(string id, string name, int score) {
    this->playerID = id;
    this->playerName = name;
    this->playerScore = score;
    this->nextNode = nullptr;
}

// Private constructor
LoserQueue::LoserQueue() {
    this->front = nullptr;
    this->size = 0;
}

// Get instance method
LoserQueue* LoserQueue::getInstance() {
    if (loserQueue == nullptr) {
        loserQueue = new LoserQueue();
    }
    return loserQueue;
}

void LoserQueue::deleteInstance(){
    if (loserQueue != nullptr) {
        delete loserQueue;
        loserQueue = nullptr;
    }
}

// Destructor
LoserQueue::~LoserQueue() {
    while (front != nullptr) {
        LoserNode* temp = front;
        front = front->nextNode;
        delete temp;
    }
    size = 0;
}

// Enqueue with priority based on score
void LoserQueue::enqueue(string playerID, string playerName, int playerScore) {
    LoserNode* newNode = new LoserNode(playerID, playerName, playerScore);

    // If empty queue or new node has higher score than front
    if (front == nullptr || playerScore > front->playerScore) {
        newNode->nextNode = front;
        front = newNode;
    }
    // Insert in the middle or at the end based on score
    else {
        LoserNode* current = front;
        while (current->nextNode != nullptr && current->nextNode->playerScore >= playerScore) {
            current = current->nextNode;
        }
        newNode->nextNode = current->nextNode;
        current->nextNode = newNode;
    }
    size++;
}

// Dequeue highest score (now just removes from front since queue is ordered)
LoserNode* LoserQueue::dequeueHighest() {
    if (front == nullptr) {
        cout << "Queue is empty!" << endl;
        return nullptr;
    }

    // Simply remove from front as it's already the highest score
    LoserNode* highest = front;
    front = front->nextNode;
    highest->nextNode = nullptr;
    size--;

    return highest;
}

//// Peek at highest score without removing
//LoserNode* LoserQueue::peekHighest() {
//    if (front == nullptr) {
//        cout << "Queue is empty!" << endl;
//        return nullptr;
//    }
//    return front;
//}

int LoserQueue::getSize()
{
    return size;
}

bool LoserQueue::isEmpty()
{
    return front == nullptr;
}

// Print queue
void LoserQueue::displayPlayers() {
    if (front == nullptr) {
        cout << "Queue is empty!" << endl;
        return;
    }

    // Print table header
    string tableLine = "----------------------------------------------------------";
    cout << tableLine << endl;
    cout << "| " << setw(10) << left << "Player ID" << " | "
        << setw(20) << left << "Name" << " | "
        << setw(8) << left << "Score" << " |" << endl;
    cout << tableLine << endl;

    // Print each player in the queue
    LoserNode* temp = front;
    while (temp != nullptr) {
        cout << "| " << setw(10) << left << temp->playerID << " | "
            << setw(20) << left << temp->playerName << " | "
            << setw(8) << left << temp->playerScore << " |" << endl;
        temp = temp->nextNode;
    }

    // Print table footer
    cout << tableLine << endl;
}

bool LoserQueue::isPlayerInQueue(string playerID)
{
    // Empty queue
    if (front == nullptr) {
        return false;
    }

    LoserNode* current = front;
    while (current != nullptr) {
        if (current->playerID == playerID) {
            return true;  // Player found
        }
        current = current->nextNode;
    }

    // Player not found
    return false;
}
