#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
using namespace std;

struct Player {
  string playerId;
  string name;
  string gender;
  string type;
  Player *next;

  Player(string playerId, string playerName, string playerGender, string type);

};

class playerList{
  string listName;
  int size;
  Player *head;
  Player *tail;



public:
  playerList(string listName);
  ~playerList();

  void addPlayer(string playerId, string playerName, string playerGender, string type);
  Player* dequeuePlayer();
  void displayPlayers();
  int getSize();
  void loadPlayer(const string& filename);

  // Getter function
  Player* getHead();

  bool isPlayerInList(string playerID);

};


#endif //PLAYER_HPP
