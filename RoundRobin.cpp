#include "RoundRobin.hpp"
#include "getDate.hpp"
#include "Score.hpp"
#include "Player.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>

using namespace std;

RoundRobin::RoundRobin(string playerID, string playerName){
    this->playerID = playerID;
    this->playerName = playerName;
    this->playerScore = 0;
    this->nextNode = nullptr;
}

rrList::rrList(string listName){
    this->listName = listName;
    this->front = nullptr;
    this->rear = nullptr;
    this->size = 0;
}

rrList::~rrList(){
    cout << listName <<" deleted" << endl;
}

void rrList::enqueue(string playerID, string playerName){
    RoundRobin * newRoundRobin = new RoundRobin(playerID, playerName);

    if(front == nullptr){
        front = newRoundRobin;
        rear = newRoundRobin;
    }
    else{
        rear->nextNode = newRoundRobin;
        rear = newRoundRobin;
    }
    rear ->nextNode=front;
    size++;
}

RoundRobin* rrList::dequeue() {
    // Check if the queue is empty
    if (front == nullptr) {
        cout << "Queue is empty! Cannot dequeue." << endl;
        return nullptr;
    }

    // Store the dequeued node
    RoundRobin* temp = front;

    // If there's only one element in the queue
    if (front == rear) {
        front = rear = nullptr;
    } else {
        // Move front pointer to the next node
        front = front->nextNode;
        rear->nextNode = front; // Maintain circular structure
    }

    temp->nextNode = nullptr; // Remove circular link for safe return
    return temp; // Return dequeued node
}


void rrList::displayRoundRobin() {
    if (rear == nullptr) {
        cout << "No matches scheduled.\n";
        return;
    }

    RoundRobin *temp = rear->nextNode; // Start from the first node
    string date = MatchStage::getMatchDate(MatchStage::ROUND_ROBIN_1);

    // Display the header
    cout << "\nRound-Robin Match Date: " << date << endl;
    cout << "=============================================\n";
    cout << "| Player 1            | Player 2            |\n";
    cout << "=============================================\n";

    do {
        cout << "| " << left << setw(20) << (temp->playerName + " (" + temp->playerID + ")")
     << "| " << left << setw(20) << (temp->nextNode->playerName + " (" + temp->nextNode->playerID + ")") << "|\n";
        temp = temp->nextNode;
    } while (temp != rear->nextNode); // Loop until we reach the starting point
    cout << "---------------------------------------------\n";
}


void rrList::scheduleRoundRobin(qualifierList &qList){
  //ofstream file("C:\\Users\\Lee Zhi Yong\\CLionProjects\\Asst_part2\\Match.csv", ios::app);
  ofstream file("Match.csv", ios::app);
  if (!file.is_open()) {
    cout << "Error opening Match.csv" << endl;
    return;
  }

  while(qList.getSize() > 0 and size <3){
    Qualifier* p1 = qList.dequeue();
    enqueue(p1->playerID,p1->playerName);
  }

  RoundRobin * current = rear;
    string date = MatchStage::getMatchDate(MatchStage::ROUND_ROBIN_1);


    do{
        string matchId= MatchStage::generateMatchID();
        // Write to CSV
        current->matchID = matchId;
    	file << matchId << ",Round-Robin, " << date << ","
                     << current->playerID << "," << current->nextNode->playerID << "\n";
    	current = current->nextNode;
    }while(current->nextNode != front);

}

void rrList::roundRobinStart(playerList* wList) {
    cout << "\nRound Robin matches start" << endl;

    RoundRobin* current = rear;

    do {
        RoundRobin* opponent = current->nextNode;

        // Check if any player has withdrawn
        bool currentWithdrawn = isPlayerWithdrawn(current->playerID, wList);
        bool opponentWithdrawn = isPlayerWithdrawn(opponent->playerID, wList);

        // If both players withdrawn
        if (currentWithdrawn && opponentWithdrawn) {
            cout << "Both players (" << current->playerName << " & " << opponent->playerName << ") withdrew. Match canceled.\n";
        }
        // If player 1 withdraw
        else if (currentWithdrawn) {
            cout << current->playerName << " (" << current->playerID << ") withdrew. "
                << opponent->playerName << " (" << opponent->playerID << ") walkovers this round and gets 1 point.\n";
            opponent->playerScore += 1;  // Give 1 point to opponent
        }
        // If player 2 withdraw
        else if (opponentWithdrawn) {
            cout << opponent->playerName << " (" << opponent->playerID << ") withdrew. "
                << current->playerName << " (" << current->playerID << ") walkovers this round and gets 1 point.\n";
            current->playerScore += 1;  // Give 1 point to current player
        }
        else {
            // Normal match if no one withdraw
            string winnerID = calculateScore(current, opponent, wList);
            if (winnerID == current->playerID) {
                cout << "Winner of This Match: " << current->playerName << "(" << current->playerID << ")" << endl << endl;
            }
            else {
                cout << "Winner of This Match: " << opponent->playerName << "(" << opponent->playerID << ")" << endl << endl;
            }
        }

        current = current->nextNode; // Move to next player
    } while (current != rear); // Complete round-robin loop

    cout << "Current scores: " << endl;

    current = front;
    do {
        cout << current->playerName << ": " << current->playerScore << endl;
        current = current->nextNode;
    } while (current != front);
}

//void rrList::displayResult() {
//
//    RoundRobin* temp = front;
//    int highest = 0, secondHighest = 0;
//
//    // Step 1: Find the highest and second-highest scores
//    do {
//        if (temp->playerScore > highest) {
//            secondHighest = highest;
//            highest = temp->playerScore;
//        } else if (temp->playerScore > secondHighest && temp->playerScore < highest) {
//            secondHighest = temp->playerScore;
//        }
//        temp = temp->nextNode;
//    } while (temp != front);
//
//    // Step 2: Display players with the highest and second-highest scores
//    cout << "\nRoundRobin Winners" << endl;
//    temp = front;
//    do {
//        if (temp->playerScore == highest || temp->playerScore == secondHighest) {
//            cout << temp->playerName << " (Score: " << temp->playerScore << ")" << endl;
//        }
//        temp = temp->nextNode;
//    } while (temp != front);
//}

void rrList::displayPlayers() {
    RoundRobin* temp = front;  // Start from the front of the queue

    // Print table header
    string tableLine = "--------------------------------";
    cout << tableLine << endl;
    cout << "| " << setw(10) << left << "Player ID" << " | "
        << setw(15) << left << "Player Name" << " |" << endl;
    cout << tableLine << endl;

    // Print player data
    do {
        cout << "| " << setw(10) << left << temp->playerID << " | "
            << setw(15) << left << temp->playerName << " |" << endl;
        temp = temp->nextNode;
    } while (temp != front);

    // Print table footer
    cout << tableLine << endl;
}

RoundRobin* rrList::getFront() {
    return front;
}

bool rrList::isPlayerWithdrawn(string& playerID, playerList* wList) {
    Player* temp = wList->getHead();
    while (temp) {
        if (temp->playerId == playerID) {
            return true;  // Player found in withdrawal list
        }
        temp = temp->next;
    }
    return false;
}