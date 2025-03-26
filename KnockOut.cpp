#include "Knockout.hpp"
#include "getDate.hpp"
#include "Score.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

Knockout::Knockout(string playerID, string playerName){
  this->playerID = playerID;
  this->playerName = playerName;
  this->nextNode = nullptr;
}

knockoutList::knockoutList(string listName){ //Queue
  this->listName = listName;
  this->front = nullptr;
  this->rear = nullptr;
  this->size = 0;
}

knockoutList::~knockoutList(){
  cout << listName <<" deleted" << endl;
}

void knockoutList::enqueue(string playerID, string playerName){ // add player to the end of queue
  Knockout* newNode = new Knockout(playerID, playerName);

  if(front == nullptr){
    front = newNode;
    rear = newNode;
  }
  else{
    rear->nextNode = newNode;
    rear = newNode;
  }
  size++;
}

Knockout *knockoutList::dequeue(){ //remove player from the front of queue
  Knockout *temp = front;
  if(front == nullptr){
    cout << listName << " empty" << endl;
    return nullptr;

  }
  else if(front == rear){
    front = rear = nullptr;

  }else{
    front = front->nextNode;
    temp->nextNode = nullptr;
  }
  size--;
  return temp;
}

//display the match schedule
void knockoutList::displayKnockout() {
  if (front == nullptr) {
    cout << "No knockout matches scheduled.\n";
    return;
  }

  Knockout *temp = front;
  // if queue have 4 player which mean it is semi-final, if have 2 player is final
  string stage = (size == 4) ? "Semi-final" : (size == 2) ? "Final" : "Knockout Stage";
  string date =  (size == 4) ? MatchStage::getMatchDate(MatchStage::SEMI_FINAL) : (size == 2) ? MatchStage::getMatchDate(MatchStage::FINAL):nullptr;


  cout << "\n" <<stage<<" Match Date: " << date << endl;
  cout << "=============================================\n";
  cout << "| Player 1            | Player 2            |\n";
  cout << "=============================================\n";

  //print player details
  while (temp != nullptr) {
    cout << "| " << left << setw(20) << (temp->playerName + " (" + temp->playerID + ")")
     << "| " << left << setw(20) << (temp->nextNode->playerName + " (" + temp->nextNode->playerID + ")") << "|\n";
    temp = temp->nextNode->nextNode;
  }
  cout << "---------------------------------------------\n";
}

// get queue size
int knockoutList::getSize(){
  return size;
}

//schedule the semi final
void knockoutList::scheduleSemi(rrList &rrListA, rrList &rrListB){
  ofstream file("Match.csv", ios::app);
  if (!file.is_open()) {
    cout << "Error opening Match.csv" << endl;
    return;
  }

  // get all the player from the queue (dequeue)
  RoundRobin *p1A = rrListA.dequeue(); //player 1 in group A round-robin
  RoundRobin *p2A = rrListA.dequeue(); //player 2 in group A round-robin
  RoundRobin *p3A = rrListA.dequeue(); //player 3 in group A round-robin

  // 1. asumme player 1 is higest score
  RoundRobin *highestA = p1A;
  RoundRobin *secondHighestA = nullptr;

  // 2. determine the highest scorer, compare with player 2
  if(p2A->playerScore > highestA->playerScore) {
    highestA = p2A;
  }

  // 3. compare with player 3
  if(p3A->playerScore > highestA->playerScore) {
    highestA = p3A;
  }

  // 4. determine the second highest scorer
  if(highestA == p1A) {
    if(p2A->playerScore>p3A->playerScore){
      secondHighestA = p2A;
    }
    else{
      secondHighestA = p3A;
    }
  }
  else if(highestA == p2A) {
    if(p1A->playerScore>p3A->playerScore){
      secondHighestA = p1A;
    }
    else{
      secondHighestA = p3A;
    }
  }
  else {
    if(p1A->playerScore>p2A->playerScore){
      secondHighestA = p1A;
    }
    else{
      secondHighestA = p2A;
    }

  }

  // 5. handle tie case (all players have same score)
  if(p1A->playerScore == p2A->playerScore && p2A->playerScore == p3A->playerScore) {
    highestA = p1A;
    secondHighestA = p2A;
  }

  // get all the player from the queue (dequeue)
  RoundRobin *p1B = rrListB.dequeue(); //player 1 in group B round-robin
  RoundRobin *p2B = rrListB.dequeue(); //player 2 in group B round-robin
  RoundRobin *p3B = rrListB.dequeue(); //player 3 in group B round-robin

  // 1. assume player 1 is highest score winner
  RoundRobin *highestB = p1B;
  RoundRobin *secondHighestB = nullptr;

  // 2. determine the highest scorer, compare with player 2
  if(p2B->playerScore > highestB->playerScore) {
    highestB = p2B;
  }

  // 3. determine the highest scorer, compare with player 3
  if(p3B->playerScore > highestB->playerScore) {
    highestB = p3B;
  }

  // 4. determine the second highest scorer
  if(highestB == p1B) {
    // If p1B is highest, compare p2B and p3B for second place
    secondHighestB = (p2B->playerScore >= p3B->playerScore) ? p2B : p3B;
  }
  else if(highestB == p2B) {
    // If p2B is highest, compare p1B and p3B for second place
    secondHighestB = (p1B->playerScore >= p3B->playerScore) ? p1B : p3B;
  }
  else { // highestB == p3B
    // If p3B is highest, compare p1B and p2B for second place
    secondHighestB = (p1B->playerScore >= p2B->playerScore) ? p1B : p2B;
  }

  // 5. handle tie case (all players have same score)
  if(p1B->playerScore == p2B->playerScore && p2B->playerScore == p3B->playerScore) {
    highestB = p1B;
    secondHighestB = p2B;
  }

    // 6. add all the winner to knockout queue
    // highest score player in each group will vs second higest score player in other group
    enqueue(highestA->playerID, highestA->playerName);
    enqueue(secondHighestB->playerID, secondHighestB->playerName);
    enqueue(highestB->playerID, highestB->playerName);
    enqueue(secondHighestA->playerID, secondHighestA->playerName);

    // 7. get the semi final match date
    string semiDate = MatchStage::getMatchDate(MatchStage::SEMI_FINAL);

    // 8. write the match in CSV
    Knockout * temp = front;

    while(temp != nullptr){
      string matchId= MatchStage::generateMatchID();
      temp->matchID = matchId;
      file << matchId << ",Semi-Final," << semiDate << ","<< temp->playerID << "," << temp->nextNode->playerID << "\n";
      temp = temp->nextNode->nextNode;
    }
}

// schedule final match
void knockoutList::scheduleFinal(){
  // 1. check file
  ofstream file("Match.csv", ios::app);
  if (!file.is_open()) {
    cout << "Error opening Match.csv" << endl;
    return;
  }

  // 2. get final match date
  Knockout * temp = front;
  string finalDate = MatchStage::getMatchDate(MatchStage::FINAL);

  // 3. write in csv
  while(temp != nullptr){
    string matchId= MatchStage::generateMatchID();
    temp->matchID = matchId;
    file << matchId << ",Final," << finalDate << ","<< temp->playerID << "," << temp->nextNode->playerID << "\n";
    temp = temp->nextNode->nextNode;
  }
}

// semi-final or final competition start
void knockoutList::knockoutStart(playerList* wList){
  // if the queue size is 2, which mean that is final match
  if (size==2){
    cout << "\nFinal match start" << endl;
  }else{
    cout << "\nSemi-Final match start" << endl;
  }

  // 1. get 2 player from the knockout queue
  Knockout* player1 = dequeue();
  Knockout* player2 = dequeue();

  // 2. check if the player 1 is withdrawn, player 2 direct win
  if (player1->playerID == "") {
      cout << "Another Player has withdrawn the match." << endl;
      cout << "Winner of This Match: " << player2->playerName << "(" << player2->playerID << ")" << endl << endl;
      enqueue(player2->playerID, player2->playerName); // enqueue the winner(player 2) back to the knockout queue
      return;
  }
  // 3. check if the player 2 is withdrawn, player 3 direct win
  else if (player2->playerID == "") {
      cout << "Another Player has withdrawn the match." << endl;
      cout << "Winner of This Match: " << player1->playerName << "(" << player1->playerID << ")" << endl << endl;
      enqueue(player1->playerID, player1->playerName); // enqueue the winner(player 1) back to the knockout queue
      return;
  }

  // 4. calculate the score of both player by calling calculateScore function
  string winnerID = calculateScore(player1->matchID, player1->playerID, player1->playerName, player2->playerID, player2->playerName, wList);

  // 5. print the winner with the score
  if (winnerID == player1->playerID) {
    cout << "Winner of This Match: " << player1->playerName << "(" << player1->playerID << ")" << endl << endl;
    enqueue(player1->playerID, player1->playerName);
  } else if (winnerID == player2->playerID) {
    cout << "Winner of This Match: " << player2->playerName << "(" << player2->playerID << ")" << endl << endl;
    enqueue(player2->playerID, player2->playerName);
  }
  // if both player dequeue, match cancel
  else {
      cout << "Both players have withdrawn. Match is cancelled.\n\n";
      enqueue("", "");  //Enqueue dummy data
  }
}

// display the chamption
void knockoutList::displayWinner(){
   cout << "\nFinal match end" << endl;
   cout << "The champion is "<< front->playerName << endl;
}

// display player that in the current match
void knockoutList::displayPlayers() {
    Knockout* temp = front;  // start from the front of the queue

    // print table header
    string tableLine = "--------------------------------";
    cout << tableLine << endl;
    cout << "| " << setw(10) << left << "Player ID" << " | "
        << setw(15) << left << "Player Name" << " |" << endl;
    cout << tableLine << endl;

    // print player data
    while (temp != nullptr) {
        cout << "| " << setw(10) << left << temp->playerID << " | "
            << setw(15) << left << temp->playerName << " |" << endl;
        temp = temp->nextNode;
    }

    // print table footer
    cout << tableLine << endl;
}

// get the front player of the queue
Knockout* knockoutList::getFront() {
    return front;
}
