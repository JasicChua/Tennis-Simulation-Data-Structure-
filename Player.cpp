#include "Player.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

Player::Player(string playerId, string playerName, string gender, string type){
  this->playerId = playerId;
  this->name = playerName;
  this->gender = gender;
  this->type = type;
  this->next = nullptr;
}

playerList::playerList(string listName){
  this->listName = listName;
  this->head = nullptr;
  this->tail = nullptr;
  this->size = 0;

}

playerList::~playerList(){
  cout << listName <<" deleted" << endl;
}

void playerList::addPlayer(string playerId, string playerName, string gender, string type){
  Player *newPlayer = new Player(playerId, playerName, gender, type);

  if(head == nullptr){
    head = newPlayer;
    tail = newPlayer;
  }
  else{
    tail->next = newPlayer;
    tail = newPlayer;

  }
  size++;

}

Player* playerList::dequeuePlayer() {
  if (head == nullptr) {
    cout << "No players available." << endl;
    return nullptr;
  }
  Player* temp = head;
  head = head->next;
  temp->next = nullptr;
  size--;
  return temp;
}




void playerList::displayPlayers() {
  Player* temp = head;

  // Print table header
  string tableLine= "-----------------------------------------------------";
  cout << tableLine << endl;
  cout << "| " << setw(8) << left << "PlayerId" << " | " << setw(15) << left << "Name" << " | " 
      << setw(6) << left << "Gender" << " | " << setw(11) << left << "Type" << " |" << endl;
  cout << tableLine << endl;

  // Print player data
  while (temp != nullptr) {
    cout << "| " << setw(8) << left << temp->playerId << " | " << setw(15) << left << temp->name << " | " 
        << setw(6) << left << temp->gender << " | " << setw(11) << left << temp->type << " |" << endl;
    temp = temp->next;
  }

  // Print table footer
  cout << tableLine << endl;
}

void playerList::loadPlayer(const  string& filename){
  ifstream file(filename);
  if(!file.is_open()){
    cout<<"File not found"<<endl;
    return;
  }

  string line;
  getline(file,line);
  while(getline(file,line)){
    stringstream ss(line);
    string playerId, name, gender, type;

    getline(ss, playerId, ',');
    getline(ss, name, ',');
    getline(ss, gender, ',');
    getline(ss, type, ',');

    // Add the player to the list
    addPlayer(playerId, name, gender, type);
  }
  file.close();
  cout << "Players loaded from " << filename << endl;

}

int playerList::getSize(){
  return size;
}

Player* playerList::getHead() {
    return head;
}

bool playerList::isPlayerInList(string playerID)
{
    Player* temp = this->head;
    while (temp != nullptr) {
        if (temp->playerId == playerID) {
            return true;  // Player found
        }
        temp = temp->next;
    }
    return false;
}

