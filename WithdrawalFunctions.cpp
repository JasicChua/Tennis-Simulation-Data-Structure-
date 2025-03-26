

#include "WithdrawalFunctions.hpp"


using namespace std;

// Show day
void showMatchDay(int matchDay) {
    cout << string(42, '-') << endl;
    cout << "| Day " << matchDay + 1 << " :   ";
    switch(matchDay){
        case 0:
            cout << setw(28) << left << "Qualifier Match Starts";
            break;
        case 1:
            cout << setw(28) << left << "Round-Robin Match A";
            break;
        case 2:
            cout << setw(28) << left << "Round-Robin Match B";
            break;
        case 3:
            cout << setw(28) << left << "Semi-Final";
            break;
        case 4:
            cout << setw(28) << left << "Final";
            break;
        default:
            cout << setw(28) << left << "Competition has ended!";
    }
    cout << " |" << endl;
    cout << string(42, '-') << endl;
}


void askForWithdrawal_BeforeQualifier(int step, playerList* sList, playerList* wList, playerList* pList) {
    string withdrawnPlayerId;
    int choice;

    do {
        pList->displayPlayers(); 
        cout << "Is there any withdrawal before Qualifier starts? (Yes = 1, No = 0): ";
        cin >> choice;
        if (choice == 1) {
            // Process withdrawal
            withdrawnPlayerId = playerWithdrawalInfo(wList);
            processWithdrawal(withdrawnPlayerId, step, sList, wList, pList);
        }
        else if (choice == 0) {
            break;
        }
        else {
            cout << "Please enter the valid number!" << endl;
        }
    } while (true);
}

// Ask withdrawal before Round Robin starts
void askForWithdrawal_BeforeRRStarts(int step, playerList* sList, playerList* wList, qualifierList* qList) {
    string withdrawnPlayerId;
    int choice;

    do {
        qList->displayPlayers();
        cout << "Is there any withdrawal before Round Robin starts? (Yes = 1, No = 0): ";
        cin >> choice;
        if (choice == 1) {
            // Process withdrawal
            withdrawnPlayerId = playerWithdrawalInfo(wList);
            processWithdrawal(withdrawnPlayerId, step, sList, wList, qList);
        }
        else if (choice == 0) {
            break;
        }
        else {
            cout << "Please enter the valid number!" << endl;
        }
    } while (true);
}

// Ask withdrawal before Round Robin B starts
void askForWithdrawal_BeforeRRB(int step, playerList* sList, playerList* wList, rrList* roundRobinB) {
    string withdrawnPlayerId;
    int choice;

    do {
        roundRobinB->displayPlayers();
        cout << "Is there any withdrawal before Round Robin B starts? (Yes = 1, No = 0): ";
        cin >> choice;
        if (choice == 1) {
            // Process withdrawal
            withdrawnPlayerId = playerWithdrawalInfo(wList);
            processWithdrawal(withdrawnPlayerId, step, sList, wList, roundRobinB);
        }
        else if (choice == 0) {
            break;
        }
        else {
            cout << "Please enter the valid number!" << endl;
        }
    } while (true);
}



template<typename ListType>
void displayOnMatchPlayers(ListType* anyList) {
    anyList->displayPlayers();
}

void displaySubstitutePlayers(playerList* sList) {
    sList->displayPlayers();
}

// Limit user letter count
string letterCountLimiter(int limit) {
    char ch;
    int chCount = 0;
    string input;

    while (true) {
        ch = _getch(); // Get character without showing it

        if (ch == '\r') {  // Enter key
            break;
        }
        else if (ch == '\b' && chCount > 0) {  // Handle backspace
            cout << "\b \b";  // Remove last character visually
            input.pop_back();
            chCount--;  
        }
        else if (chCount < limit) {  // Only allow input within limit
            cout << ch;  // Show character
            input += ch;
            chCount++;  
        }
    }

    return input;
}

string displayLatestMatch(string& playerId) {
    ifstream file("Match.csv");
    if (!file.is_open()) {
        return "File 'Match.csv' not found";
    }

    string line, latestMatchId, latestMatchName, latestMatchDate;
    bool found = false;

    // Skip header line
    //getline(file, line);

    // Read match info
    while (getline(file, line)) {
        stringstream ss(line);
        string matchId, matchName, date, player1, player2;

        // Parse CSV line
        getline(ss, matchId, ',');
        getline(ss, matchName, ',');
        getline(ss, date, ',');
        getline(ss, player1, ',');
        getline(ss, player2, ',');

        // Check if playerId is in this match
        if (player1 == playerId || player2 == playerId) {
            // Display latest match details
            cout << "Latest Match:";
            cout << "\n  - Match ID: " << matchId << endl;
            cout << "  - Match Name: " << matchName << endl;
            cout << "  - Match Date: " << date << endl;
            found = true;
            break;
        }

    }
    file.close();

    // If no match assigned for the player
    if (!found)
    {
        cout << "There is no match assigned for " << playerId << " for now." << endl;;
        return "N/A";
    }

    return latestMatchId;
}

void writeWithdrawal(string playerId, string reason) {

    ofstream file("Withdrawal.csv", ios::app); 
    if (!file.is_open()) {
        cout << "File not found" << endl;
        return;
    }
    
    // Write to CSV
    file << playerId << "," << reason << ",";   // Still have replacement player need to fill in + \n

    file.close();
}

string getNameFromID(string playerId) {
    ifstream file("Player.csv");
    if (!file.is_open()) {
        return "File 'Player.csv' not found";
    }

    string line, playerName;

    // Skip header line
    getline(file, line);

    // Read match info
    while (getline(file, line)) {
        stringstream ss(line);
        string ID, name, gender, type;

        // Parse CSV line
        getline(ss, ID, ',');
        getline(ss, name, ',');
        getline(ss, gender, ',');
        getline(ss, type, ',');
        
        if (ID == playerId) {
            playerName = name;
        }
    }
    file.close();

    return playerName;
}



string playerWithdrawalInfo(playerList* wList) {
    while (true) {
        cout << "\n" << string(40, '-') << endl;
        cout << "|            Withdrawal Page           |" << endl;
        cout << string(40, '-') << endl;
        int option;
        string playerId, playerName, reason;

        // Ask PlayerID
        cout << "Enter Player ID: ";
        cin.ignore();
        getline(cin, playerId);

        // Get player name
        playerName = getNameFromID(playerId);

        // Use loserQueue
        LoserQueue* loserQueue = LoserQueue::getInstance();

        if (playerName == "") {
            cout << "Player Not Found!" << endl;
        }
        else if (wList->isPlayerInList(playerId) || loserQueue->isPlayerInQueue(playerId)) {
            cout << "Player has withdrawn or been eliminated!" << endl;
        }
        else {
            // Let user confirm information
            cout << string(48, '-') << endl;
            cout << "Player ID      : " << playerId << endl;
            cout << "Player Name    : " << playerName << endl;

            // Validate integer input
            while (true) {
                cout << "Is the information correct? (Yes = 1, No = 0): ";
                cin >> option;

                if (cin.fail() || (option != 0 && option != 1)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
                    cout << "Invalid input! Please enter (Yes = 1, No = 0)." << endl;
                }
                else {
                    break;
                }
            }

            if (option == 1) {
                // User input reason
                const int letterCountLimit = 20;
                cout << "Please enter withdrawal reason (" << letterCountLimit << " letters max): ";
                cin.ignore();
                reason = letterCountLimiter(letterCountLimit);
                writeWithdrawal(playerId, reason);
                cout << "\nPlayer withdrawal recorded to CSV." << endl;
                return playerId;
            }
            continue;  // If user selects No, loop again
        }

        // Validate integer input for retry option
        while (true) {
            cout << "Try again? (Yes = 1, No = 0): ";
            cin >> option;

            if (cin.fail() || (option != 0 && option != 1)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter (Yes = 1, No = 0)." << endl;
            }
            else {
                break;
            }
        }

        if (option == 0) {
            cout << "Back to Withdrawal Menu..." << endl;
            return "";
        }
        cin.ignore();
    }
}



template<typename ListType>
void processWithdrawal(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, ListType* anyList) {
    string substitutePlayerId;
    // If no player id entered in the previous function
    if (withdrawnPlayerId == "")
        return;

    // Before Qualifier = Assign substitute player
    else if (matchCounter == -1) {
        playerList* pList = (playerList*)anyList;
        Player* current = pList->getHead();
        Player* playerNode = nullptr;

        // Find the withdrawn player
        while (current != nullptr) {
            if (current->playerId == withdrawnPlayerId) {
                playerNode = current;
                break;
            }
            current = current->next;
        }

        // Get the first player in Substitute queue
        Player* substitutePlayer = sList->getHead();

        // Add withdrawn player into Withdrawn queue
        wList->addPlayer(playerNode->playerId, playerNode->name, playerNode->gender, playerNode->type);
        
        // Perform substitution
        playerNode->playerId = substitutePlayer->playerId;
        playerNode->name = substitutePlayer->name;
        playerNode->gender = substitutePlayer->gender;
        playerNode->type = substitutePlayer->type;
        substitutePlayerId = substitutePlayer->playerId;

        // Dequeue the used substitute
        sList->dequeuePlayer();  
        cout << "Player substituted successfully! " << substitutePlayer->name << "(" << substitutePlayer->playerId <<") has replaced " << withdrawnPlayerId << "." << endl;
    }

    // During Qualifier = Opponent win the game 
    else if (matchCounter == 0) {
        qualifierList* qList = (qualifierList*)anyList;
        Qualifier* current = qList->getFront();
        Qualifier* playerNode = nullptr;

        while (current != nullptr) {
            if (current->playerID == withdrawnPlayerId) {
                playerNode = current;
                break;
            }
            current = current->nextNode;
        }

        // Add withdrawn player into Withdrawn queue
        wList->addPlayer(playerNode->playerID, playerNode->playerName, "NULL", "NULL");
        cout << "Player has withdrawn on match day and will be eliminated immediately." << endl;
    }

    // Before Round Robin = Assign Highest score loser 
    else if (matchCounter == 1 && (typeid(*anyList) == typeid(qualifierList))) {
        qualifierList* qList = (qualifierList*)anyList;
        Qualifier* current = qList->getFront();
        Qualifier* playerNode = nullptr;

        while (current != nullptr) {
            if (current->playerID == withdrawnPlayerId) {
                playerNode = current;
                break;
            }
            current = current->nextNode;
        }

        wList->addPlayer(playerNode->playerID, playerNode->playerName, "NULL", "NULL");


        // Get the singleton instance of LoserQueue
        LoserQueue* loserQueue = LoserQueue::getInstance();

        // Check if LoserQueue is empty
        if (loserQueue->isEmpty()) {
            // Get the first player in Substitute queue
            Player* substitutePlayer = sList->getHead();

            // Perform substitution with substitute player
            playerNode->playerID = substitutePlayer->playerId;
            playerNode->playerName = substitutePlayer->name;
            substitutePlayerId = substitutePlayer->playerId;

            // Dequeue the used substitute
            sList->dequeuePlayer();

            cout << "Player substituted successfully! Substitute player" << substitutePlayer->name
                << "(" << substitutePlayer->playerId << ") has replaced "
                << withdrawnPlayerId << "." << endl;
        }
        else {
            // Get the highest scored loser player
            LoserNode* loserPlayer = loserQueue->dequeueHighest();

            // Perform substitution with loser player
            playerNode->playerID = loserPlayer->playerID;
            playerNode->playerName = loserPlayer->playerName;
            substitutePlayerId = loserPlayer->playerID;

            cout << "Player substituted successfully! Eliminated player " << loserPlayer->playerName
                << "(" << loserPlayer->playerID << ") has replaced "
                << withdrawnPlayerId << "." << endl;

            // Free memory
            delete loserPlayer;
        }
    }
    // Before Round Robin B = Assign Highest score loser 
    else if (matchCounter == 1 && (typeid(*anyList) == typeid(rrList))) {
        rrList* RRBList = (rrList*)anyList;
        RoundRobin* current = RRBList->getFront();
        RoundRobin* playerNode = nullptr;

        while (current != nullptr) {
            if (current->playerID == withdrawnPlayerId) {
                playerNode = current;
                break;
            }
            current = current->nextNode;
        }

        wList->addPlayer(playerNode->playerID, playerNode->playerName, "NULL", "NULL");

        // Get the singleton instance of LoserQueue
        LoserQueue* loserQueue = LoserQueue::getInstance();

        // Check if LoserQueue is empty
        if (loserQueue->isEmpty()) {
            // Get the first player in Substitute queue
            Player* substitutePlayer = sList->getHead();

            // Perform substitution with substitute player
            playerNode->playerID = substitutePlayer->playerId;
            playerNode->playerName = substitutePlayer->name;
            substitutePlayerId = substitutePlayer->playerId;

            // Dequeue the used substitute
            sList->dequeuePlayer();

            cout << "Player substituted successfully! Substitute player" << substitutePlayer->name
                << "(" << substitutePlayer->playerId << ") has replaced "
                << withdrawnPlayerId << "." << endl;
        }
        else {
            // Get the highest scored loser player
            LoserNode* loserPlayer = loserQueue->dequeueHighest();

            // Perform substitution with loser player
            playerNode->playerID = loserPlayer->playerID;
            playerNode->playerName = loserPlayer->playerName;
            substitutePlayerId = loserPlayer->playerID;

            cout << "Player substituted successfully! Eliminated player " << loserPlayer->playerName
                << "(" << loserPlayer->playerID << ") has replaced "
                << withdrawnPlayerId << "." << endl;

            // Free memory
            delete loserPlayer;
        }
    }
    // During Round Robin A and B = Other player add score 
    else if (matchCounter == 2 || matchCounter == 3) {
        rrList* RRList = (rrList*)anyList;
        RoundRobin* current = RRList->getFront();
        RoundRobin* playerNode = nullptr;

        while (current != nullptr) {
            if (current->playerID == withdrawnPlayerId) {
                playerNode = current;
                break;
            }
            current = current->nextNode;
        }

        // Count number of withdrawn players that in the schedule
        int withdrawnCount = 0;
        withdrawnCount++;   // Add current player to count 

        current = RRList->getFront();
        do{
            // Check if this player is in the withdrawn list
            if (wList->isPlayerInList(current->playerID)) {
                withdrawnCount++;
            }
            current = current->nextNode;
        } while (current != RRList->getFront());

        wList->addPlayer(playerNode->playerID, playerNode->playerName, "NULL", "NULL");


        // If 2 or more players have withdrawn, substitute
        if (withdrawnCount >= 2) {
            // Get the singleton instance of LoserQueue
            LoserQueue* loserQueue = LoserQueue::getInstance();

            // Check if LoserQueue is empty
            if (loserQueue->isEmpty()) {
                // Get the first player in Substitute queue
                Player* substitutePlayer = sList->getHead();

                // Perform substitution with substitute player
                playerNode->playerID = substitutePlayer->playerId;
                playerNode->playerName = substitutePlayer->name;
                substitutePlayerId = substitutePlayer->playerId;

                // Dequeue the used substitute
                sList->dequeuePlayer();

                wList->addPlayer(playerNode->playerID, playerNode->playerName, "NULL", "NULL");

                cout << "Player substituted successfully! Substitute player" << substitutePlayer->name
                    << "(" << substitutePlayer->playerId << ") has replaced "
                    << withdrawnPlayerId << "." << endl;
            }
            else {
                // Get the highest scored loser player
                LoserNode* substituteLoser = loserQueue->dequeueHighest();

                // Perform substitution with loser player
                playerNode->playerID = substituteLoser->playerID;
                playerNode->playerName = substituteLoser->playerName;
                substitutePlayerId = substituteLoser->playerID;

                cout << "Player substituted successfully! Eliminated player" << substituteLoser->playerName
                    << "(" << substituteLoser->playerID << ") has replaced withdrawn player "
                    << withdrawnPlayerId << " for match operation." << endl;

                // Free the memory for the dequeued node
                delete substituteLoser;
            }
        }
        else {
            // Less than 2 withdrawals, just add to withdrawn queue and continue with fewer players
            wList->addPlayer(playerNode->playerID, playerNode->playerName, "NULL", "NULL");
            cout << "Player has withdrawn on match day. " << "1 player has withdrawn, match will continue with remaining players." << endl;
        }
    }
    // Knockout = Another player win the game  -> Make sure how counting marks works first
    else if (matchCounter == 4 || matchCounter == 5) {
        knockoutList* kList = (knockoutList*)anyList;
        Knockout* current = kList->getFront();
        Knockout* playerNode = nullptr;

        while (current != nullptr) {
            if (current->playerID == withdrawnPlayerId) {
                playerNode = current;
                break;
            }
            current = current->nextNode;
        }

        // Add withdrawn player into Withdrawn queue
        wList->addPlayer(playerNode->playerID, playerNode->playerName, "NULL", "NULL");
        cout << "Player has withdrawn on match day and will be eliminated immediately." << endl;
    }
    // Continue write withdrawal
    ofstream file("Withdrawal.csv", ios::app);
    if (!file.is_open()) {
        cout << "File not found" << endl;
        return;
    }

    // Write remaining info (subsitute player id) that previous function has not added yet
    if (substitutePlayerId == "")
        file << "N\A" << "\n";
    else
        file << substitutePlayerId << "\n";  

    file.close();

}


// Template function instantiation
template void processWithdrawal<playerList>(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, playerList*);
template void processWithdrawal<qualifierList>(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, qualifierList*);
template void processWithdrawal<rrList>(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, rrList*);
template void processWithdrawal<knockoutList>(string withdrawnPlayerId, int matchCounter, playerList* sList, playerList* wList, knockoutList*);

template void displayOnMatchPlayers<playerList>(playerList*);
template void displayOnMatchPlayers<qualifierList>(qualifierList*);
template void displayOnMatchPlayers<rrList>(rrList*);
template void displayOnMatchPlayers<knockoutList>(knockoutList*);