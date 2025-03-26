#include <iostream>
#include "Player.hpp"
#include "Qualifier.hpp"
#include "RoundRobin.hpp"
#include "Knockout.hpp"
#include "WithdrawalFunctions.hpp"
#include "LoserQueue.hpp"
#include "Score.hpp"
#include "TicketSystem.hpp"

#include <fstream>

using namespace std;

void competitionMenu();

// Reset all CSV
static void resetCSV() {
    // Reset Match.csv
    ofstream matchFile("Match.csv", ios::trunc);
    matchFile.close();

    // Reset Withdrawal.csv
    ofstream withdrawalFile("Withdrawal.csv", ios::out); // Open in write mode
    if (!withdrawalFile.is_open()) {
        cout << "Failed to open the file!" << endl;
        return;
    }
    withdrawalFile << "Player ID,Reason,Replacement_Player ID" << endl;

    withdrawalFile.close();

    // Reset Point.csv
    ofstream scoreFile("Point.csv", ios::out); // Open in write mode
    if (!scoreFile.is_open()) {
        cout << "Failed to open the file!" << endl;
        return;
    }
    scoreFile << "PointID,GameID,Player1Score,Player2Score" << endl;
    scoreFile.close(); 

    // Reset Game.csv
    ofstream gameFile("Game.csv", ios::out); // Open in write mode
    if (!gameFile.is_open()) {
        cout << "Failed to open the file!" << endl;
        return;
    }
    gameFile << "GameID,SetID,Player1Game,Player2Game" << endl;
    gameFile.close(); 

    // Reset Set.csv
    ofstream setFile("Set.csv", ios::out); // Open in write mode
    if (!setFile.is_open()) {
        cout << "Failed to open the file!" << endl;
        return;
    }
    setFile << "SetID,MatchID,Player1SetScore,Player2SetScore" << endl;
    setFile.close(); 
}

static void categorizePlayer(playerList* fullList, playerList* pList, playerList* sList) {
    while (fullList->getSize() != 0) {
        Player* current = fullList->dequeuePlayer();
        if (current->type == "Participant") {
            pList->addPlayer(current->playerId, current->name, current->gender, current->type);
        }
        else if (current->type == "Substitute") {
            sList->addPlayer(current->playerId, current->name, current->gender, current->type);
        }
        current = current->next;
        delete current;
    }
    cout << "Players categorized into respective queues." << endl;
}



int main() {
    // Initialize ticket system
    TicketSystem system;

    resetCSV();
    int step = -1; // Track competition phase
    int choice;
    string withdrawnPlayerId;

    // Initialize lists
    playerList *fullList = new playerList("Tournament Players");
    playerList *pList = new playerList("Participant Queue");
    playerList *sList = new playerList("Substitute Queue");
    playerList *wList = new playerList("Withdrawn Players");
    MatchList* matchList = new MatchList();

    qualifierList *qList = new qualifierList("Qualifier Matches");
    rrList *roundRobinA = new rrList("Round Robin Matches Group A");
    rrList *roundRobinB = new rrList("Round Robin Matches Group B");
    knockoutList *kList = new knockoutList("Knockout Matches");

    //string filePath = "C:\\Users\\Lee Zhi Yong\\CLionProjects\\Asst_part2\\Player.csv";
    string filePath = "Player.csv";

    // Load player from csv and categorize
    fullList->loadPlayer(filePath);
    categorizePlayer(fullList, pList, sList);


    // Ask for any withdrawals before Qualifier starts
    askForWithdrawal_BeforeQualifier(step, sList, wList, pList);
    step++;


    // Schedule players for Qualifier
    qList->scheduleQualifier(*pList);
    cout << "\n\nAll participants are scheduled for Qualifiers.\n";


    do {
        cout << "\n============ Competition Menu ============\n";
        showMatchDay(step);
        cout << "0. Start Competition\n";
        cout << "1. Display Schedule\n";
        cout << "2. Display Ongoing Match Players\n";
        cout << "3. Display Available Substitute Players\n";
        cout << "4. Player Withdrawal\n";
        cout << "5. Ticket\n";
        cout << "6. Show Detail History Match Record\n";
        cout << "7. Participant Performance Report\n";
        if (step != 0 && step < 3) {
            cout << "8. Display Eliminated Players\n";
        }
        cout << "99. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 0:
            // Move to the next step if all matches in the current step are completed
                if (step == 0) {
                    qList->qualifiersStart(wList);
                    qList->qualifiersStart(wList);
                    qList->qualifiersStart(wList);
                    qList->qualifiersStart(wList);
                    qList->qualifiersStart(wList);
                    qList->qualifiersStart(wList);

                    // Ask for any withdrawals before Round Robin starts
                    askForWithdrawal_BeforeRRStarts(step+1, sList, wList, qList);  //matchCounter = 1

                    roundRobinA->scheduleRoundRobin(*qList);
                    roundRobinB->scheduleRoundRobin(*qList);
                    step++;
                    cout << "Qualifier matches completed! Moving to Round Robin.\n";
                } else if (step ==1) {
                    roundRobinA->roundRobinStart(wList);
                    cout << "Round Robin A matches completed! Moving to Round Robin B matches.\n";
                    // Ask for any withdrawals before Round Robin starts
                    askForWithdrawal_BeforeRRB(step, sList, wList, roundRobinB);  //matchCounter = 1
                    step++;

                } else if (step ==2) {
                    roundRobinB->roundRobinStart(wList);
                    kList->scheduleSemi(*roundRobinA,*roundRobinB);
                    step++;
                    cout << "Round Robin B matches completed! Moving to Semi-Finals.\n";
                    cout << "No early withdrawal allowed!\n";

                } else if (step == 3) {
                    kList->knockoutStart(wList);
                    kList->knockoutStart(wList);
                    kList->scheduleFinal();
                    step++;
                    cout << "Semi-Final matches completed! Moving to Finals.\n";
                    cout << "No early withdrawal allowed!\n";

                } else if (step == 4) {
                    kList->knockoutStart(wList);
                    kList->displayWinner();
                    step++;
                    cout << "Final match completed! Tournament finished.\n";

                }else{
                    cout << "Tournament finished.\n";
                }
                break;
            case 1: // Display Schedule
                if (step == 0) {
                    qList->displayQualifier();
                } else if (step == 1) {
                    roundRobinA->displayRoundRobin();
                }else if (step == 2) {
                    roundRobinB->displayRoundRobin();
                }
                else if (step == 3) {
                    kList->displayKnockout();
                } else if (step == 4) {
                    kList->displayKnockout();
                } else {
                    cout << "Tournament has ended!\n";
                }
                break;

            case 2: // Display Ongoing Match Players
                cout << "Displaying current match players...\n";

                if (step == 0) {  // Qualifier stage
                    displayOnMatchPlayers(qList);
                }
                else if (step == 1) {  // Round Robin stage
                    cout << "Group A:\n";
                    displayOnMatchPlayers(roundRobinA);
                }
                else if (step == 2) {  // Round Robin stage
                    cout << "Group B:\n";
                    displayOnMatchPlayers(roundRobinB);
                }
                else if (step == 3 || step == 4) {  // Knockout stage (Semi-Final & Final)
                    displayOnMatchPlayers(kList);
                }
                else {
                    cout << "No matches currently in progress.\n";
                }
                break;

            case 3: // Display Available Substitute Players
                if (step < 5) {
                    cout << "Displaying substitute players...\n";
                    sList->displayPlayers();
                }
                else {
                    cout << "Tournament has ended. No players left.\n";
                }

                break;

            case 4: // Player Withdrawal

                // If no substitute is available
                if (sList->getHead() == nullptr) {
                    cout << "No available substitutes in the queue. Aborting process..." << endl;
                    continue;
                }
                else if (step == 5) {
                    cout << "Tournament has ended. Withdrawal cannot be done again.\n";
                    continue;
                }

                cout << "Processing player withdrawal...\n";
                withdrawnPlayerId = playerWithdrawalInfo(wList);
                if (step == 0) {
                    processWithdrawal(withdrawnPlayerId, step, sList, wList, qList);   //matchCounter = 0
                }
                else if (step == 1) {
                    processWithdrawal(withdrawnPlayerId, step + 1, sList, wList, roundRobinA);  //matchCounter = 2
                }
                else if (step == 2) {
                    processWithdrawal(withdrawnPlayerId, step + 1, sList, wList, roundRobinB);  //matchCounter = 3
                }
                else if (step == 3 || step == 4) {
                    processWithdrawal(withdrawnPlayerId, step + 1, sList, wList, kList);  //matchCounter = 4,5
                }
                break;

            case 5: // Ticket (ignores step)
                cout << "Processing ticket purchase...\n";
                int choice;
                do
                {
                    cout << "\nTicket System Menu\n";
                    cout << "1. Buy Ticket\n";
                    cout << "2. Simulate Spectator Flow\n";
                    cout << "3. Exit\n";
                    cout << "Please enter choice(1/2/3): ";
                    cin >> choice;

                    // Validate user input
                    if (!system.checkInt())
                    {
                        continue;
                    }

                    switch (choice)
                    {
                    case 1:
                        system.buyTicket();
                        break;
                    case 2:
                        system.simulateSpectatorFlow();
                        break;
                    case 3:
                        cout << "Exiting...\n";
                        break;
                    default:
                        cout << "Invalid choice. Try again.\n";
                    }
                } while (choice != 3);
                break;
            case 6: {
                // Display Match History
                // Read Player.csv
                playerList* participantList = new playerList("Participant List");

                //ifstream playerFile("C:\\Backup Assignment\\Data Structure Part 2\\Player.csv");
                ifstream playerFile("Player.csv");
                if (!playerFile.is_open()) {
                    cout << "Error opening Player.csv" << endl;
                    break;
                }

                string line;
                // Skip Header
                getline(playerFile, line);
                // Read and store relevant set data which match the required MatchID
                while (getline(playerFile, line)) {
                    stringstream ss(line);
                    string playerID, name, gender, type;

                    getline(ss, playerID, ',');
                    getline(ss, name, ',');
                    getline(ss, gender, ',');
                    getline(ss, type, '\n');

                    participantList->addPlayer(playerID, name, gender, type);
                }
                playerFile.close();

                // Read Match.csv
                ifstream matchFile("Match.csv");
                if (!matchFile.is_open()) {
                    cout << "Error opening Match.csv" << endl;
                    break;
                }
                // Read and store relevant set data which match the required MatchID
                while (getline(matchFile, line)) {
                    stringstream ss(line);
                    string matchID, stage, date, player1ID, player2ID;

                    getline(ss, matchID, ',');
                    getline(ss, stage, ',');
                    getline(ss, date, ',');
                    getline(ss, player1ID, ',');
                    getline(ss, player2ID, '\n');

                    Player* temp = participantList->getHead();
                    Player* player1 = nullptr;
                    Player* player2 = nullptr;
                    while (temp != nullptr) {
                        // cout << player1ID << "vs" << temp->playerId << endl;
                        if (player1ID == temp->playerId) {
                            player1 = temp;
                        }
                        // cout << player2ID << "vs" << temp->playerId << endl;
                        if (player2ID == temp->playerId) {
                            player2 = temp;
                        }
                        if (player1 != nullptr && player2 != nullptr) {
                            break;
                        }
                        temp = temp->next;
                    }
                    // cout << "Found match: " << matchID << " with players " << player1ID << " and " << player2ID << endl;
                    // if (!player1)
                    //     cout << "Player " << player1ID << " not found in player list." << endl;
                    // if (!player2)
                    //     cout << "Player " << player2ID << " not found in player list." << endl;

                    matchList->insertEnd(matchID, player1, player2);
                }
                matchFile.close();

                int validMatch = 0;
                // Display Available Match ID
                if (step == 0) {
                    cout << "Match Have Not Start Yet!!!" << endl;
                    break;
                }
                else if (step == 1) {
                    validMatch = 6;
                }
                else if (step == 2) {
                    validMatch = 9;
                }
                else if (step == 3) {
                    validMatch = 12;
                }
                else if (step == 4) {
                    validMatch = 14;
                }
                else {
                    validMatch = 15;
                }
                matchIDSelectionMenu(validMatch, *matchList);
                // User Choose Match ID
                int inputNumber;
                bool validInput = false;

                do {
                    cout << "Please Select an Option: ";
                    if (!(cin >> inputNumber)) {
                        cout << "Invalid input. Please enter a valid number." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    // Validate the number based on 'step'
                    bool inRange = false;
                    if (step == 0) {
                        break;
                    }
                    else if (step == 1) {
                        inRange = (inputNumber >= 1 && inputNumber <= 6);
                    }
                    else if (step == 2) {
                        inRange = (inputNumber >= 1 && inputNumber <= 9);
                    }
                    else if (step == 3) {
                        inRange = (inputNumber >= 1 && inputNumber <= 12);
                    }
                    else if (step == 4) {
                        inRange = (inputNumber >= 1 && inputNumber <= 14);
                    }
                    else {
                        inRange = (inputNumber >= 1 && inputNumber <= 15);
                    }

                    if (!inRange) {
                        cout << "Invalid option. Please choose a number within the valid range." << endl;
                        continue; // input was valid as an int but out of range; ask again
                    }

                    // If we reached here, the input is valid and in range.
                    validInput = true;
                } while (!validInput);

                string searchMatchID = generateMatchID(inputNumber);

                MatchNode* searchNode = matchList->getHead();
                while (searchNode != nullptr) {
                    if (searchMatchID == searchNode->MatchID) {
                        break;
                    }
                    searchNode = searchNode->next;
                }
                // Display
                displaySpecificMatchHistory(searchNode->MatchID, searchNode->Player1, searchNode->Player2);
                break;
            }

            case 7:
                if (step == 0) {
                    cout << "Match Have Not Start Yet!!!" << endl;
                    break;
                }
                else {
                    displayPerformanceReport();
                }
                break;

            case 8:
                if (step != 0 && step < 3) {
                    LoserQueue* loserQueue = LoserQueue::getInstance();
                    loserQueue->displayPlayers();
                }
                else {
                    cout << "Invalid choice! Try again.\n";
                }
                break;
            case 99:
                cout << "Returning to Main Menu...\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }



    } while (choice != 99);

    delete pList;
    delete sList;
    delete wList;
    delete qList;
    delete roundRobinA;
    delete roundRobinB;
    delete kList;
    LoserQueue::deleteInstance();
    return 0;
}


