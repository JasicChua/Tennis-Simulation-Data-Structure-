#include "Qualifier.hpp"
#include "getDate.hpp"
#include "Score.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

Qualifier::Qualifier(string matchID, string playerID, string playerName){
  this->matchID = matchID;
  this->playerID = playerID;
  this->playerName = playerName;
  this->nextNode = nullptr;
}

qualifierList:: qualifierList(string listName){
  this->listName = listName;
  this->front = nullptr;
  this->rear = nullptr;
  this->size = 0;
}

qualifierList::~qualifierList(){
  cout << listName <<" deleted" << endl;
}

void qualifierList::enqueue(string matchID, string playerID,string playerName){
  Qualifier * newQualifiers = new Qualifier(matchID, playerID, playerName);

  if(front == nullptr){
    front = newQualifiers;
    rear = newQualifiers;
  }
  else{
    rear->nextNode = newQualifiers;
    rear = newQualifiers;
    }
  size++;
}

Qualifier* qualifierList::dequeue(){
  Qualifier * temp = front;
  if(front == nullptr){
    cout << " empty queue" << endl;
    return nullptr;
  }
  else if(front == rear){
    front = rear = nullptr;
    cout << "After dequeue, empty queue" << endl;

  }else{
      front = front->nextNode;
      temp->nextNode = nullptr;
  }
  size--;
  return temp;
}

void qualifierList::displayQualifier() {
  if (front == nullptr) {
    cout << "No qualifier matches scheduled.\n";
    return;
  }

  Qualifier *temp = front;
  string date = MatchStage::getMatchDate(MatchStage::QUALIFIER);

  // Display the header
  cout << "\nQualifier Match Date: " << date << endl;
  cout << "=============================================\n";
  cout << "| Player 1            | Player 2            |\n";
  cout << "=============================================\n";

  // Print matchups in a structured format
  while (temp != nullptr && temp->nextNode != nullptr) {
    cout << "| " << left << setw(20) << (temp->playerName + " (" + temp->playerID + ")")
         << "| " << left << setw(20) << (temp->nextNode->playerName + " (" + temp->nextNode->playerID + ")") << "|\n";
    temp = temp->nextNode->nextNode; // Move to the next pair
  }
  cout << "---------------------------------------------\n";
}


void qualifierList::scheduleQualifier(playerList &pList){
  //ofstream file("C:\\Users\\Lee Zhi Yong\\CLionProjects\\Asst_part2\\Match.csv", ios::app);
  ofstream file("Match.csv", ios::app);
  if (!file.is_open()) {
    cout << "Error opening Match.csv" << endl;
    return;
  }


  string date = MatchStage::getMatchDate(MatchStage::QUALIFIER);


  while (pList.getSize() >= 2 and size<=11) { // Ensure at least two players
    Player* p1 = pList.dequeuePlayer();
    Player* p2 = pList.dequeuePlayer();

    if (!p1 || !p2) break; // Safety check

    // Generate unique match ID
    string matchId= MatchStage::generateMatchID();
    // Enqueue match into qualifierList
    enqueue(matchId,p1->playerId,p1->name);
    enqueue(matchId,p2->playerId,p2->name);

    // Write to CSV
    file << matchId << ",Qualifier," << date << ","
         << p1->playerId << "," << p2->playerId << "\n";

    delete p1;
    delete p2;
  }

  file.close();
}

void qualifierList::qualifiersStart(playerList* wList) {
  cout << "\nQualifier match start" << endl;
  Qualifier* player1 = dequeue();
  Qualifier* player2 = dequeue();

  // Calculate Point
  string winnerID = calculateScore(player1->matchID, player1->playerID, player1->playerName, player2->playerID, player2->playerName, wList);

  if (winnerID == player1->playerID) {
    cout << "Winner of This Match: " << player1->playerName << "(" << player1->playerID << ")" << endl << endl;
    enqueue("",player1->playerID, player1->playerName);
  } else if (winnerID == player2->playerID) {
    cout << "Winner of This Match: " << player2->playerName << "(" << player2->playerID << ")" << endl << endl;
    enqueue("",player2->playerID, player2->playerName);
  }
  else {
      cout << "Both players have withdrawn. Match Cancelled!" << endl << endl;
  }
}

void qualifierList::displayResult(){
  Qualifier * temp = front;
  cout << "\nQualifier Winner: " << endl;
  while(temp != nullptr){
    cout <<temp->playerID<< endl;
    temp = temp->nextNode;
  }

}

int qualifierList::getSize(){
  return size;
}

void qualifierList::displayPlayers() {
    Qualifier* temp = front;  // Start from the front of the queue

    // Print table header
    string tableLine = "--------------------------------";
    cout << tableLine << endl;
    cout << "| " << setw(10) << left << "Player ID" << " | "
        << setw(15) << left << "Player Name" << " |" << endl;
    cout << tableLine << endl;

    // Print player data
    while (temp != nullptr) {
        cout << "| " << setw(10) << left << temp->playerID << " | "
            << setw(15) << left << temp->playerName << " |" << endl;
        temp = temp->nextNode;
    }

    // Print table footer
    cout << tableLine << endl;
}

Qualifier* qualifierList::getFront() {
    return front;
}

