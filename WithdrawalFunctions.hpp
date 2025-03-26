#ifndef WITHDRAWALFUNCTIONS_HPP
#define WITHDRAWALFUNCTIONS_HPP

#include "Player.hpp"
#include "Knockout.hpp"
#include "Qualifier.hpp"
#include "RoundRobin.hpp"
#include "LoserQueue.hpp"

#include <iostream>
#include <iomanip>
#include <conio.h>  
#include <fstream>
#include <sstream>
#include <limits>  


using namespace std;

void showMatchDay(int matchCounter);

void askForWithdrawal_BeforeQualifier(int step, playerList* sList, playerList* wList, playerList* pList);
void askForWithdrawal_BeforeRRStarts(int step, playerList* sList, playerList* wList, qualifierList* qList);
void askForWithdrawal_BeforeRRB(int step, playerList* sList, playerList* wList, rrList* roundRobinB);

template<typename ListType>
void displayOnMatchPlayers(ListType* anyList);

void displaySubstitutePlayers(playerList* sList);
string letterCountLimiter(int limit);
string displayLatestMatch(string& playerId);
void writeWithdrawal(string playerId, string reason);
string getNameFromID(string playerId);
string playerWithdrawalInfo(playerList* wList);

template<typename ListType>
void processWithdrawal(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, ListType* anyList);

// Explicit template instantiations
extern template void processWithdrawal<playerList>(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, playerList*);
extern template void processWithdrawal<qualifierList>(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, qualifierList*);
extern template void processWithdrawal<rrList>(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, rrList*);
extern template void processWithdrawal<knockoutList>(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, knockoutList*);

extern template void displayOnMatchPlayers<playerList>(playerList*);
extern template void displayOnMatchPlayers<qualifierList>(qualifierList*);
extern template void displayOnMatchPlayers<rrList>(rrList*);
extern template void displayOnMatchPlayers<knockoutList>(knockoutList*);

#endif // WITHDRAWALFUNCTIONS_HPP
