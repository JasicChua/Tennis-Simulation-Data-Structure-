#include "Score.hpp"

// Helper function to generate IDs
string generateMatchID(int matchCount) {
    stringstream ss;
    ss << "M" << setw(3) << setfill('0') << matchCount;
    return ss.str();
}

string generateSetID(int setCount) {
    stringstream ss;
    ss << "S" << setw(3) << setfill('0') << setCount;
    return ss.str();
}

string generateGameID(int gameCount) {
    stringstream ss;
    ss << "G" << setw(3) << setfill('0') << gameCount;
    return ss.str();
}

string generatePointID(int pointCount) {
    stringstream ss;
    ss << "P" << setw(4) << setfill('0') << pointCount;
    return ss.str();
}

string scoreToString(PointScore score) {
    switch (score) {
    case LOVE:
        return "LOVE";
    case FIFTEEN:
        return "15";
    case THIRTY:
        return "30";
    case FORTY:
        return "40";
    case GAME:
        return "GAME";
    case ADVANTAGE:
        return "ADV";
    default:
        return "Error";
    }
}

// Score Queue Implementation
ScoreQueue::ScoreQueue() {
    front = nullptr;
    rear = nullptr;
    size = 0;
    currentGameID = "";
}

ScoreQueue::~ScoreQueue() {
    ScoreNode* current = front;
    while (current) {
        ScoreNode* temp = current->next;
        delete current;
        current = temp;
    }
    front = nullptr;
    rear = nullptr;
}

bool ScoreQueue::isEmpty() {
    return front == nullptr;
}

void ScoreQueue::enqueue(string scoreID, string gameID, int score1, int score2) {
    ScoreNode* newNode = new ScoreNode(scoreID, gameID, score1, score2);
    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;
}

ScoreNode* ScoreQueue::dequeue() {
    if (isEmpty())
        return nullptr;
    ScoreNode* temp = front;
    front = front->next;
    if (front == nullptr)
        rear = nullptr;

    size--;
    return temp;
}

void ScoreQueue::displayScore() {
    cout << "Score Queue Node" << endl;
    ScoreNode* current = front;
    while (current) {
        cout << current->ScoreID << ", " << current->GameID << ", " << current->score1 << ", " << current->score2 << endl;
        current = current->next;
    }
}

void ScoreQueue::setcurrentGameID(string gameID) {
    currentGameID = gameID;
}
ScoreNode* ScoreQueue::getFront() {
    return front;
}
ScoreNode* ScoreQueue::getRear() {
    return rear;
}
int ScoreQueue::getSize() {
    return size;
}
string ScoreQueue::getCurrentGameID() {
    return currentGameID;
}


GameQueue::GameQueue() {
    front = nullptr;
    rear = nullptr;
    currentSetID = "";
    size = 0;
}

GameQueue::~GameQueue() {
    GameNode* current = front;
    while (current) {
        GameNode* next = current->next;
        delete current;
        current = next;
    }
    front = nullptr;
    rear = nullptr;
}

bool GameQueue::isEmpty() {
    return front == nullptr;
}

void GameQueue::enqueue(string gameID, string setID, int p1, int p2) {
    GameNode* newNode = new GameNode(gameID, setID, p1, p2);

    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;
}

GameNode* GameQueue::dequeue() {
    if (isEmpty())
        return nullptr;

    GameNode* temp = front;
    front = front->next;

    if (front == nullptr)
        rear = nullptr;

    size--;
    return temp;
}

void GameQueue::displayGame() {
    cout << "Game Queue Node" << endl;
    GameNode* temp = front;
    while (temp) {
        cout << temp->GameID << ", " << temp->SetID << ", " << temp->p1Game << ", " << temp->p2Game << endl;
        temp = temp->next;
    }
}

void GameQueue::setCurrentSetID(string setID) {
    currentSetID = setID;
}
string GameQueue::getCurrentSetID() {
    return currentSetID;
}
GameNode* GameQueue::getFront() {
    return front;
}
GameNode* GameQueue::getRear() {
    return rear;
}
int GameQueue::getSize() {
    return size; // Note: This seems odd in your header, might want to check if this should return int instead
}


SetQueue::SetQueue() {
    front = nullptr;
    rear = nullptr;
    size = 0;
    currentMatchID = "";
}

SetQueue::~SetQueue() {
    SetNode* current = front;
    while (current) {
        SetNode* next = current->next;
        delete current;
        current = next;
    }
    front = nullptr;
    rear = nullptr;
}

bool SetQueue::isEmpty() {
    return front == nullptr;
}

void SetQueue::enqueue(string setID, string matchID, int p1Set, int p2Set) {
    SetNode* newNode = new SetNode(setID, matchID, p1Set, p2Set);

    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;
}

SetNode* SetQueue::dequeue() {
    if (isEmpty())
        return nullptr;

    SetNode* temp = front;
    front = front->next;

    if (front == nullptr)
        rear = nullptr;

    size--;
    return temp;
}

void SetQueue::displaySet() {
    cout << "Set Queue Node" << endl;
    SetNode* current = front;
    int setIndex = 1;
    while (current) {
        cout << current->SetID << ", " << current->MatchID << ", " << current->p1Set << ", " << current->p2Set << endl;
        current = current->next;
    }
}

void SetQueue::setCurrentMatchID(string matchID) {
    currentMatchID = matchID;
}
string SetQueue::getCurrentMatchID() {
    return currentMatchID;
}
SetNode* SetQueue::getFront() {
    return front;
}
SetNode* SetQueue::getRear() {
    return rear;
}
int SetQueue::getSize() {
    return size; // Note: This seems odd in your header, might want to check if this should return int instead
}

MatchList::MatchList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}
MatchList::~MatchList() {
    MatchNode* current = head;
    while (current != nullptr) {
        MatchNode* temp = current;
        current = current->next;
        delete temp;
    }
}

void MatchList::insertEnd(string matchID, Player* player1, Player* player2) {
    MatchNode* newNode = new MatchNode(matchID, player1, player2);
    if (head == nullptr) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

MatchNode* MatchList::getHead() {
    return head;
}
MatchNode* MatchList::getTail() {
    return tail;
}
int MatchList::getSize() {
    return size;
}

// Modified to handle multiple matches
void saveAllData(SetQueue& setQueue, GameQueue& gameQueue, ScoreQueue& scoreQueue) {
    ofstream appendSetFile("Set.csv", ios::app);
    SetNode* currentSet;
    while ((currentSet = setQueue.dequeue()) != nullptr) {
        appendSetFile << currentSet->SetID << "," << currentSet->MatchID << ","
            << currentSet->p1Set << "," << currentSet->p2Set << endl;
        delete currentSet; // Free memory!
    }
    appendSetFile.close();

    ofstream appendGameFile("Game.csv", ios::app);
    GameNode* currentGame;
    while ((currentGame = gameQueue.dequeue()) != nullptr) {
        appendGameFile << currentGame->GameID << "," << currentGame->SetID << ","
            << currentGame->p1Game << "," << currentGame->p2Game << endl;
        delete currentGame; // Free memory!
    }
    appendGameFile.close();

    ofstream appendPointFile("Point.csv", ios::app);
    ScoreNode* currentScore;
    while ((currentScore = scoreQueue.dequeue()) != nullptr) {
        appendPointFile << currentScore->ScoreID << "," << currentScore->GameID << ","
            << currentScore->score1 << "," << currentScore->score2 << endl;
        delete currentScore; // Free memory!
    }
    appendPointFile.close();
}

string calculateScore(string matchID, string player1ID, string player1Name, string player2ID, string player2Name, playerList* wList) {
    bool p1Withdrawn = false, p2Withdrawn = false;

    // Before start counting score, check if there is player withdrawn
    Player* withdrawnPlayer = wList->getHead();
    while (withdrawnPlayer != nullptr) {
        if (withdrawnPlayer->playerId == player1ID) {
            p1Withdrawn = true;
        }
        if (withdrawnPlayer->playerId == player2ID) {
            p2Withdrawn = true;
        }
        withdrawnPlayer = withdrawnPlayer->next;
    }

    SetQueue setQueue;
    GameQueue gameQueue;
    ScoreQueue scoreQueue;

    int totalSetWinsP1 = 0, totalSetWinsP2 = 0;
    int p1Score = 0, p2Score = 0;             // Track total points for each player
    int p1PointsWon = 0, p2PointsWon = 0;     // Track individual points won
    int p1GamesWon = 0, p2GamesWon = 0;       // Track total games won

    if (p1Withdrawn && p2Withdrawn) {
        cout << "Both players " << player1Name << " (" << player1ID << ") and "
            << player2Name << " (" << player2ID << ") have withdrawn. Match is cancelled.\n\n";
    }
    else if (p1Withdrawn) {
        cout << "Player " << player1Name << " (" << player1ID << ") has withdrawn.\n";
        cout << "Opponent " << player2Name << " (" << player2ID << ") wins by walkover!\n\n";
        totalSetWinsP2 = 2;
    }
    else if (p2Withdrawn) {
        cout << "Player " << player2Name << " (" << player2ID << ") has withdrawn.\n";
        cout << "Opponent " << player1Name << " (" << player1ID << ") wins by walkover!\n\n";
        totalSetWinsP1 = 2;
    }
    else {
        srand(steady_clock::now().time_since_epoch().count()); // Ensure random time
        // Simulate a best-of-3 sets match (first to 2 sets wins)
        while (totalSetWinsP1 < 2 && totalSetWinsP2 < 2) {
            int p1GameWins = 0, p2GameWins = 0;
            setCount++; // Increment set counter
            string currentSetID = generateSetID(setCount);


            gameQueue.setCurrentSetID(currentSetID); // Set the Set ID for this game list

            // New set rules:
            // - Regular win: First to 3 games
            // - Deuce situation: First to 4 games (with at least 2 game advantage)
            while ((p1GameWins < 3 && p2GameWins < 3) ||
                (abs(p1GameWins - p2GameWins) < 2 && p1GameWins < 4 && p2GameWins < 4)) {

                int p1Score = 0, p2Score = 0;
                gameCount++; // Increment game counter
                string currentGameID = generateGameID(gameCount);


                scoreQueue.setcurrentGameID(currentGameID);

                // Simulate a game until one player wins
                while (true) {
                    if (rand() % 2) {
                        // Player 1 wins the point
                        p1PointsWon++; // Count individual points

                        if (p1Score == PointScore::FORTY && p2Score == PointScore::FORTY)   // Check 40:40
                            p1Score = PointScore::ADVANTAGE;
                        else if (p1Score == PointScore::ADVANTAGE)                          // Check ADV:40
                            p1Score = PointScore::GAME;
                        else if (p2Score == PointScore::ADVANTAGE) {                        // Check 40:ADV
                            p1Score = PointScore::FORTY;
                            p2Score = PointScore::FORTY;
                        }
                        else
                            p1Score++;    // None deuce situation
                    }
                    else {
                        // Player 2 wins the point
                        p2PointsWon++; // Count individual points

                        if (p1Score == PointScore::FORTY && p2Score == PointScore::FORTY)   // Check 40:40
                            p2Score = PointScore::ADVANTAGE;
                        else if (p2Score == PointScore::ADVANTAGE)                          // Check 40:ADV
                            p2Score = PointScore::GAME;
                        else if (p1Score == PointScore::ADVANTAGE) {                        // Check 40:ADV
                            p1Score = PointScore::FORTY;
                            p2Score = PointScore::FORTY;
                        }
                        else
                            p2Score++;                           // None deuce situation
                    }
                    pointCount++; // Increment point counter
                    string currentPointID = generatePointID(pointCount);
                    scoreQueue.enqueue(currentPointID, currentGameID, p1Score, p2Score);

                    if (p1Score == PointScore::GAME || p2Score == PointScore::GAME) {
                        break;
                    }
                }

                // Determine game winner and update cumulative games
                if (p1Score == PointScore::GAME) {
                    p1GameWins++;
                    p1GamesWon++; // Track total games won
                }
                else if (p2Score == PointScore::GAME) {
                    p2GameWins++;
                    p2GamesWon++; // Track total games won
                }

                // Enqueue the game with the game counts
                gameQueue.enqueue(currentGameID, currentSetID, p1GameWins, p2GameWins);
            }

            // Update total set wins for match result
            if (p1GameWins > p2GameWins) {
                totalSetWinsP1++;
            }
            else {
                totalSetWinsP2++;
            }

            setQueue.enqueue(currentSetID, matchID, totalSetWinsP1, totalSetWinsP2);
        }
            //    sets.displaySet();
        // Save all data to files
        saveAllData(setQueue, gameQueue, scoreQueue);
    }
    

    // Display overall match results (overall set wins) and each set's details.
    cout << "Match Results (" << matchID <<"): " << endl;
    cout << "=======================================================" << endl;
    cout << "| " << left << setw(25) << (player1Name + " (" + player1ID + ")")
         << "| " << left << setw(25) << (player2Name + " (" + player2ID + ")") << "|" << endl;
    cout << "=======================================================" << endl;
    cout << "| Win Sets: " << left << setw(15) << totalSetWinsP1
         << "| Win Sets: " << left << setw(15) << totalSetWinsP2 << "|" << endl;
    cout << "| Win Games: " << left << setw(14) << p1GamesWon
         << "| Win Games: " << left << setw(14) << p2GamesWon << "|" << endl;
    cout << "| Win Points: " << left << setw(13) << p1PointsWon
         << "| Win Points: " << left << setw(13) << p2PointsWon << "|" << endl;
    cout << "=======================================================" << endl;

    // Get singleton instance of LoserQueue
    LoserQueue* loserQueue = LoserQueue::getInstance();

    // Determine winner and enqueue loser to LoserQueue
    string winnerId;
    if (p1Withdrawn && p2Withdrawn) {
        return "NONE";
    }
    else if (totalSetWinsP1 == 2) {
        // Player 1 wins
        winnerId = player1ID;
        int LScore = p2GamesWon * 5 + p2PointsWon;
        loserQueue->enqueue(player2ID, player2Name, LScore);
    }
    else {
        // Player 2 wins
        winnerId = player2ID;
        int LScore = p1GamesWon * 5 + p1PointsWon;
        loserQueue->enqueue(player1ID, player1Name, LScore);
    }

    return winnerId;
}


string calculateScore(RoundRobin* player1, RoundRobin* player2, playerList* wList) {
    srand(steady_clock::now().time_since_epoch().count()); // Ensure random time

    SetQueue setQueue;
    GameQueue gameQueue;
    ScoreQueue scoreQueue;

    int totalSetWinsP1 = 0, totalSetWinsP2 = 0;
    int p1Score = 0, p2Score = 0;             // Track total points for each player
    int p1PointsWon = 0, p2PointsWon = 0;     // Track individual points won
    int p1GamesWon = 0, p2GamesWon = 0;       // Track total games won

    // Simulate a best-of-3 sets match (first to 2 sets wins)
    while (totalSetWinsP1 < 2 && totalSetWinsP2 < 2) {
        int p1GameWins = 0, p2GameWins = 0;
        setCount++; // Increment set counter
        string currentSetID = generateSetID(setCount);


        gameQueue.setCurrentSetID(currentSetID); // Set the Set ID for this game list

        // New set rules:
        // - Regular win: First to 3 games
        // - Deuce situation: First to 4 games (with at least 2 game advantage)
        while ((p1GameWins < 3 && p2GameWins < 3) ||
               (abs(p1GameWins - p2GameWins) < 2 && p1GameWins < 4 && p2GameWins < 4)) {

            int p1Score = 0, p2Score = 0;
            gameCount++; // Increment game counter
            string currentGameID = generateGameID(gameCount);


            scoreQueue.setcurrentGameID(currentGameID);

            // Simulate a game until one player wins
            while (true) {
                if (rand() % 2){
                    // Player 1 wins the point
                    p1PointsWon++; // Count individual points

                    if (p1Score == PointScore::FORTY && p2Score == PointScore::FORTY)   // Check 40:40
                        p1Score = PointScore::ADVANTAGE;
                    else if (p1Score == PointScore::ADVANTAGE)                          // Check ADV:40
                        p1Score = PointScore::GAME;
                    else if (p2Score == PointScore::ADVANTAGE) {                        // Check 40:ADV
                        p1Score = PointScore::FORTY;
                        p2Score = PointScore::FORTY;
                    }
                    else
                        p1Score++;    // None deuce situation
                }
                else {
                    // Player 2 wins the point
                    p2PointsWon++; // Count individual points

                    if (p1Score == PointScore::FORTY && p2Score == PointScore::FORTY)   // Check 40:40
                        p2Score = PointScore::ADVANTAGE;
                    else if (p2Score == PointScore::ADVANTAGE)                          // Check 40:ADV
                        p2Score = PointScore::GAME;
                    else if (p1Score == PointScore::ADVANTAGE) {                        // Check 40:ADV
                        p1Score = PointScore::FORTY;
                        p2Score = PointScore::FORTY;
                    }
                    else
                        p2Score++;                           // None deuce situation
                }
                pointCount++; // Increment point counter
                string currentPointID = generatePointID(pointCount);
                scoreQueue.enqueue(currentPointID, currentGameID, p1Score, p2Score);

                if (p1Score == PointScore::GAME || p2Score == PointScore::GAME) {
                    break;
                }
            }

            // Determine game winner and update cumulative games
            if (p1Score == PointScore::GAME) {
                p1GameWins++;
                p1GamesWon++; // Track total games won
            } else if (p2Score == PointScore::GAME) {
                p2GameWins++;
                p2GamesWon++; // Track total games won
            }

            // Enqueue the game with the game counts
            gameQueue.enqueue(currentGameID, currentSetID, p1GameWins, p2GameWins);
        }

        // Update total set wins for match result
        if (p1GameWins > p2GameWins) {
            totalSetWinsP1++;
        } else {
            totalSetWinsP2++;
        }

        setQueue.enqueue(currentSetID, player1->matchID, totalSetWinsP1, totalSetWinsP2);
    }

    // Calculate Score for Round Robin
    // - Win one set: 100,000
    // - Win one game: 1,000
    // - Win one point: 1
    p1Score = (totalSetWinsP1 * 100000) + (p1GamesWon * 1000) + p1PointsWon;
    p2Score = (totalSetWinsP2 * 100000) + (p2GamesWon * 1000) + p2PointsWon;

    // Display overall match results (overall set wins) and each set's details.
    cout << "Match Results (" << player1->matchID <<"): " << endl;
    cout << "=======================================================" << endl;
    cout << "| " << left << setw(25) << (player1->playerName + " (" + player1->playerID + ")")
         << "| " << left << setw(25) << (player2->playerName + " (" + player2->playerID + ")") << "|" << endl;
    cout << "=======================================================" << endl;
    cout << "| Win Sets: " << left << setw(15) << totalSetWinsP1
         << "| Win Sets: " << left << setw(15) << totalSetWinsP2 << "|" << endl;
    cout << "| Win Games: " << left << setw(14) << p1GamesWon
         << "| Win Games: " << left << setw(14) << p2GamesWon << "|" << endl;
    cout << "| Win Points: " << left << setw(13) << p1PointsWon
         << "| Win Points: " << left << setw(13) << p2PointsWon << "|" << endl;
    cout << "| Total Score: " << left << setw(12) << p1Score
         << "| Total Score: " << left << setw(12) << p2Score << "|" << endl;
    cout << "=======================================================" << endl;

    // Save all data to files
    saveAllData(setQueue, gameQueue, scoreQueue);

    // Update player scores
    player1->playerScore += p1Score;
    player2->playerScore += p2Score;

    if (totalSetWinsP1 == 2)
        return player1->playerID;
    return player2->playerID;
}

void loadResultCSV(string& matchID, SetQueue& setQueue, GameQueue& gameQueue, ScoreQueue& scoreQueue) {
    // Read Set.csv
    ifstream setFile("Set.csv");
    if (!setFile.is_open()) {
        cout << "Error opening Set.csv" << endl;
        return;
    }

    // Skip header line
    string line;
    getline(setFile, line);

    // Read and store relevant set data which match the required MatchID
    while (getline(setFile, line)) {
        stringstream ss(line);
        string setID, MID, p1SetScore, p2SetScore;

        getline(ss, setID, ',');
        getline(ss, MID, ',');
        getline(ss, p1SetScore, ',');
        getline(ss, p2SetScore, '\n');

        // Only Add to queue when matches matchID
        if (MID == matchID) {
            setQueue.enqueue(setID, matchID, stoi(p1SetScore), stoi(p2SetScore));
        }
    }
    setFile.close();

    // setQueue.displaySet();

    if (setQueue.isEmpty()) {
        cout << "No match found with ID: " << matchID << endl;
        return;
    }

    // Read Game.csv
    ifstream gameFile("Game.csv");
    if (!gameFile.is_open()) {
        cout << "Error opening Game.csv" << endl;
        return;
    }

    // Skip header line
    getline(gameFile, line);

    // Read and store relevant game data which match the required MatchID
    while (getline(gameFile, line)) {
        stringstream ss(line);
        string gameID, setID, p1Game, p2Game;

        getline(ss, gameID, ',');
        getline(ss, setID, ',');
        getline(ss, p1Game, ',');
        getline(ss, p2Game, '\n');

        bool belongsToSet = false;
        SetNode* current = setQueue.getFront();

        for (int i = 0; i < setQueue.getSize(); i++) {
            if (current->SetID == setID) {
                belongsToSet = true;
                break;
            }
            current = current->next;
        }
        if (belongsToSet) {
            gameQueue.enqueue(gameID, setID, stoi(p1Game), stoi(p2Game));
        }
    }
    gameFile.close();

    //    gameQueue.displayGame();

        // Read Point.csv
    ifstream pointFile("Point.csv");
    if (!pointFile.is_open()) {
        cout << "Error opening Point.csv" << endl;
        return;
    }

    // Skip header line
    getline(pointFile, line);

    // Read and store relevant point data which match the required MatchID
    while (getline(pointFile, line)) {
        stringstream ss(line);
        string pointID, gameID, p1Score, p2Score;

        getline(ss, pointID, ',');
        getline(ss, gameID, ',');
        getline(ss, p1Score, ',');
        getline(ss, p2Score, '\n');

        bool belongsToGame = false;
        GameNode* current = gameQueue.getFront();

        for (int i = 0; i < gameQueue.getSize(); i++) {
            if (current->GameID == gameID) {
                belongsToGame = true;
                break;
            }
            current = current->next;
        }
        if (belongsToGame) {
            scoreQueue.enqueue(pointID, gameID, stoi(p1Score), stoi(p2Score));
        }
    }
    pointFile.close();

    //    scoreQueue.displayScore();
}


void displaySpecificMatchHistory(string matchID, Player* player1, Player* player2) {
    SetQueue setQueue;
    GameQueue gameQueue;
    ScoreQueue scoreQueue;

    loadResultCSV(matchID, setQueue, gameQueue, scoreQueue);

    SetNode* temp = setQueue.getRear();

    // Print match header
    // cout << "Final Match Results: " << player1->name  << " vs " << player2->name << endl;
    // cout << player1->name << " " << temp->p1Set << " - " << temp->p2Set << " " << player2->name << endl;

    cout << "Match Results (" << matchID << "): " << endl;
    cout << "=======================================================" << endl;
    cout << "| " << left << setw(25) << (player1->name + " (" + player1->playerId + ")")
        << "| " << left << setw(25) << (player2->name + " (" + player2->playerId + ")") << "|" << endl;
    cout << "=======================================================" << endl;
    cout << "| Win Sets: " << left << setw(15) << temp->p1Set
        << "| Win Sets: " << left << setw(15) << temp->p2Set << "|" << endl;
    cout << "=======================================================" << endl;


    // Display set flow
    string setResults = "{";
    SetNode* currentSet = setQueue.getFront();
    while (currentSet != nullptr) {
        setResults += to_string(currentSet->p1Set) + "-" + to_string(currentSet->p2Set);
        if (currentSet->next != nullptr) {
            setResults += ", ";
        }
        currentSet = currentSet->next;
    }
    setResults += "}";

    cout << "\nFinal Match Sets: " << setResults << endl;

    // Display each set
    currentSet = setQueue.getFront();
    int setNumber = 1;

    while (currentSet != nullptr) {
        cout << endl << "Set " << setNumber << " (Set ID: " << currentSet->SetID << ", Match ID: " << currentSet->MatchID
            << ") Result: " << endl;

        // Display all games in this set
        GameNode* currentGame = gameQueue.getFront();
        int gameNumber = 1;

        while (currentGame != nullptr) {
            if (currentGame->SetID == currentSet->SetID) {
                cout << " -> Game " << gameNumber << " (Game ID: " << currentGame->GameID << ") Result: ["
                    << currentGame->p1Game << " - " << currentGame->p2Game << "]" << endl;

                // Display all point history for this game
                cout << "    - Point History: ";
                ScoreNode* currentScore = scoreQueue.getFront();
                bool firstPoint = true;

                while (currentScore != nullptr) {
                    if (currentScore->GameID == currentGame->GameID) {
                        if (!firstPoint) {
                            cout << " ";
                        }
                        cout << "(" << scoreToString(static_cast<PointScore>(currentScore->score1)) << "-"
                            << scoreToString(static_cast<PointScore>(currentScore->score2)) << ")";
                        firstPoint = false;
                    }
                    currentScore = currentScore->next;
                }
                cout << endl;
                gameNumber++;
            }
            currentGame = currentGame->next;
        }
        setNumber++;
        currentSet = currentSet->next;
    }
}

void matchIDSelectionMenu(int validMatch, MatchList& matchList) {
    MatchNode* current = matchList.getHead();
    if (!current) {
        cout << "No matches available." << endl;
        return;
    }

    int count = 1;
    // Use a while loop so that if the actual list is shorter than validMatch, we don't crash.
    cout << "Select MatchID for Display Detail Match Record" << endl;
    while (current != nullptr && count <= validMatch) {
        // Check if player pointers are valid before dereferencing.
        if (current->Player1 && current->Player2) {
            cout << count << ". M00" << count << " -> "
                << current->Player1->name << "(" << current->Player1->playerId << ") - "
                << current->Player2->name << "(" << current->Player2->playerId << ")" << endl;
        }
        current = current->next;
        count++;
    }
}


void displayPerformanceReport() {
    // Read CSV (Player,Match,Set)
    playerList* participantList = new playerList("Participant List");
    MatchList* matchList = new MatchList();
    SetQueue* setQueue = new SetQueue();


    //ifstream playerFile("C:\\Backup Assignment\\Data Structure Part 2\\Player.csv");
    ifstream playerFile("Player.csv");
    if (!playerFile.is_open()) {
        cout << "Error opening Player.csv" << endl;
        return;;
    }

    string line;
    getline(playerFile, line);
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
        return;;
    }
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
        matchList->insertEnd(matchID, player1, player2);
    }
    matchFile.close();


    // Read Set.csv
    ifstream setFile("Set.csv");
    if (!setFile.is_open()) {
        cout << "Error opening Set.csv" << endl;
        return;;
    }
    getline(setFile, line);
    while (getline(setFile, line)) {
        stringstream ss(line);
        string setID, matchID, P1Score, P2Score;
        getline(ss, setID, ',');
        getline(ss, matchID, ',');
        getline(ss, P1Score, ',');
        getline(ss, P2Score, '\n');

        int p1Score = stoi(P1Score);
        int p2Score = stoi(P2Score);
        setQueue->enqueue(setID, matchID, p1Score, p2Score);
    }
    setFile.close();

    // setQueue->displaySet();

    // Process Data (Count Win set,game, total set, game and win rate)
    cout << endl << "--- Performance Report ---" << endl;
    cout << string(113, '=') << endl;
    cout << left
        << "| " << setw(8) << "PlayerID"
        << " | " << setw(12) << "Name"
        << " | " << setw(14) << "Total Matches"
        << " | " << setw(10) << "Match Wins"
        << " | " << setw(13) << "Match Rate (%)"
        << " | " << setw(10) << "Total Sets"
        << " | " << setw(8) << "Set Wins"
        << " | " << setw(11) << "Set Rate (%) |";
    cout << endl << string(113, '=') << endl;

    // Iterate through each player in participant list.
    Player* player = participantList->getHead();
    while (player != nullptr) {
        int matchesPlayed = 0;
        int matchWins = 0;
        int setsPlayed = 0;
        int setWins = 0;

        // Iterate through all matches.
        MatchNode* match = matchList->getHead();
        while (match != nullptr) {
            // Check if the player is involved in this match.
            bool isInMatch = false;
            bool isPlayer1 = false;
            if (match->Player1 && match->Player1->playerId == player->playerId) {
                isInMatch = true;
                isPlayer1 = true;
            }
            else if (match->Player2 && match->Player2->playerId == player->playerId) {
                isInMatch = true;
            }
            if (isInMatch) {
                matchesPlayed++;
                int player1SetWins = 0;
                int player2SetWins = 0;
                // Scan the set queue for sets belonging to this match.
                SetNode* set = setQueue->getFront();
                while (set != nullptr) {
                    if (set->MatchID == match->MatchID) {
                        // Count sets won for each side.
                        if (set->p1Set > set->p2Set)
                            player1SetWins++;
                        else if (set->p2Set > set->p1Set)
                            player2SetWins++;
                    }
                    set = set->next;
                }
                // Update sets played and set wins for the player.
                int totalSets = player1SetWins + player2SetWins;
                setsPlayed += totalSets;
                if (isPlayer1)
                    setWins += player1SetWins;
                else
                    setWins += player2SetWins;

                // Determine the match winner.
                if (player1SetWins > player2SetWins) {
                    if (isPlayer1) matchWins++;
                }
                else if (player2SetWins > player1SetWins) {
                    if (!isPlayer1) matchWins++;
                }
            }
            match = match->next;
        }

        // Calculate win rates.
        double matchWinRate = (matchesPlayed > 0) ? (100.0 * matchWins / matchesPlayed) : 0.0;
        double setWinRate = (setsPlayed > 0) ? (100.0 * setWins / setsPlayed) : 0.0;

        cout << left
            << "| " << setw(8) << player->playerId
            << " | " << setw(12) << player->name
            << " | " << setw(14) << matchesPlayed
            << " | " << setw(10) << matchWins
            << " | " << setw(14) << fixed << setprecision(2) << matchWinRate
            << " | " << setw(10) << setsPlayed
            << " | " << setw(8) << setWins
            << " | " << setw(12) << fixed << setprecision(2) << setWinRate << " | " << endl;
        player = player->next;
    }
    cout << string(113, '=') << endl;
}