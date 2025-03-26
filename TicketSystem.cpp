#include "TicketSystem.hpp"
#include "getDate.hpp"
using namespace std;
#include <iomanip>
#include <ctime>

// Constructor
TicketSystem::TicketSystem() : head(nullptr), matchCount(0), ticketCounts(nullptr) 
{
    // Load existing tickets to get current counts
    loadTicketData();
}

// Destructor
TicketSystem::~TicketSystem() 
{
    while (head) 
    {
        Ticket* temp = head;
        head = head->next;
        delete temp;
    }

    while (ticketCounts) 
    {
        MatchTickets* temp = ticketCounts;
        ticketCounts = ticketCounts->next;
        delete temp;
    }
}

//Validate Integer Input
bool TicketSystem::checkInt()
{
    if (cin.fail())
    {
        cin.clear();  // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
        system("cls");
        cout << "Invalid input. Please enter a number.\n\n";
        return false;
    }
    return true;
}

//Check whether the date is today's date or future dates
bool TicketSystem::isDateTodayOrFuture(const string& date) 
{
    try {
        // Parse the date string (format: YYYY-MM-DD)
        stringstream dateStream(date);
        string yearStr, monthStr, dayStr;

        // Check if the string has the correct format
        if (date.length() < 10 || date[4] != '-' || date[7] != '-')
        {
            cout << "Invalid date format: " << date << endl;
            return false;
        }

        // Parse the date components
        yearStr = date.substr(0, 4);
        monthStr = date.substr(5, 2);
        dayStr = date.substr(8, 2);

        // Convert to integers with error checking
        int year, month, day;
        try 
        {
            year = stoi(yearStr);
            month = stoi(monthStr);
            day = stoi(dayStr);
        }
        catch (const exception&) 
        {
            cout << "Error converting date parts to integers: " << date << endl;
            return false;
        }

        // Get current date using safer methods
        time_t now = time(0);
        tm timeinfo = {};

        // Use localtime_s on Windows
#ifdef _WIN32
        if (localtime_s(&timeinfo, &now) != 0) 
        {
            cout << "Error getting local time" << endl;
            return false;
        }
#else
    // For non-Windows platforms
        if (localtime_r(&now, &timeinfo) == NULL) 
        {
            cout << "Error getting local time" << endl;
            return false;
        }
#endif

        int currentYear = 1900 + timeinfo.tm_year;
        int currentMonth = 1 + timeinfo.tm_mon;
        int currentDay = timeinfo.tm_mday;

        // Compare dates
        if (year > currentYear) return true;
        if (year == currentYear && month > currentMonth) return true;
        if (year == currentYear && month == currentMonth && day >= currentDay) return true;

        return false;
    }
    catch (const exception& e) 
    {
        cout << "Exception in isDateTodayOrFuture: " << e.what() << endl;
        return false;
    }
}

// boolean for checking today's date
bool TicketSystem::isToday(const string& date) 
{
    try 
    {
        // Parse the date string (format: YYYY-MM-DD)
        string yearStr = date.substr(0, 4);
        string monthStr = date.substr(5, 2);
        string dayStr = date.substr(8, 2);

        // Convert to integers
        int year = stoi(yearStr);
        int month = stoi(monthStr);
        int day = stoi(dayStr);

        // Get current date
        time_t now = time(0);
        tm timeinfo = {};

#ifdef _WIN32
        if (localtime_s(&timeinfo, &now) != 0) 
        {
            return false;
        }
#else
        if (localtime_r(&now, &timeinfo) == NULL) 
        {
            return false;
        }
#endif

        int currentYear = 1900 + timeinfo.tm_year;
        int currentMonth = 1 + timeinfo.tm_mon;
        int currentDay = timeinfo.tm_mday;

        // Check if it's today exactly
        return (year == currentYear && month == currentMonth && day == currentDay);
    }
    catch (const std::exception& e) 
    {
        cout << "Exception in isToday: " << e.what() << endl;
        return false;
    }
}


//Check if the date is future date
bool TicketSystem::isFutureDate(const string& date) 
{
    try 
    {
        // Parse the date string (format: YYYY-MM-DD)
        string yearStr = date.substr(0, 4);
        string monthStr = date.substr(5, 2);
        string dayStr = date.substr(8, 2);

        // Convert to integers
        int year = stoi(yearStr);
        int month = stoi(monthStr);
        int day = stoi(dayStr);

        // Get current date
        time_t now = time(0);
        tm timeinfo = {};

#ifdef _WIN32
        if (localtime_s(&timeinfo, &now) != 0) 
        {
            return false;
        }
#else
        if (localtime_r(&now, &timeinfo) == NULL) {
            return false;
        }
#endif

        int currentYear = 1900 + timeinfo.tm_year;
        int currentMonth = 1 + timeinfo.tm_mon;
        int currentDay = timeinfo.tm_mday;

        // Check if date is in the future (not today)
        if (year > currentYear) return true;
        if (year == currentYear && month > currentMonth) return true;
        if (year == currentYear && month == currentMonth && day > currentDay) return true;

        return false;
    }
    catch (const std::exception& e) 
    {
        cout << "Exception in isFutureDate: " << e.what() << endl;
        return false;
    }
}

string TicketSystem::generateCustomerID() 
{
    ifstream file("Ticket.csv");
    if (!file) 
    {
        return "C001";  // Start from C001 if no file exists
    }

    string lastID = "C000"; // Default before reading
    string line;

    while (getline(file, line)) 
    {
        stringstream ss(line);
        string matchDate, customerID, customerName, ticketTypeStr;

        //Read match date and customerID from Ticket.csv
        getline(ss, matchDate, ',');
        getline(ss, customerID, ',');

        if (!customerID.empty()) 
        {
            lastID = customerID;  // Store the last read customer ID
        }
    }
    file.close();

    // Extract the number part (such as "C003" -> 3)
    int num = stoi(lastID.substr(1)) + 1;

    // Format it back to CXXX
    ostringstream newID;
    newID << "C" << setw(3) << setfill('0') << num;

    return newID.str();
}

//insert tickets in ascending order by priority( VIP -> EarlyBird -> General)
void TicketSystem::insertTicketSorted(Ticket* newTicket) 
{
    // If linked list is empty or new ticket has a higher priority
    if (!head || newTicket->ticketType < head->ticketType) 
    {
        // Insert at the beginning if it's the highest priority
        newTicket->next = head;
        head = newTicket;
    }
    else 
    {
        Ticket* current = head;
        // Move forward as long as the next node exists and has a ticketType 
        // less than or equal to the new ticket’s type
        while (current->next && current->next->ticketType <= newTicket->ticketType) 
        {
            current = current->next;
        }
        newTicket->next = current->next;
        current->next = newTicket;
    }
}

//Update ticket count for each ticket type
void TicketSystem::updateTicketCount(const string& matchDate, int ticketType) 
{
    // Find match date node or create if not exists
    MatchTickets* curr = ticketCounts;
    MatchTickets* prev = nullptr;

    while (curr && curr->matchDate != matchDate) 
    {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) 
    {
        // Create new node for this match date
        MatchTickets* newNode = new MatchTickets
        {
            matchDate, 0, 0, 0, nullptr
        };

        if (!prev) 
        {
            ticketCounts = newNode;
        }
        else 
        {
            prev->next = newNode;
        }
        curr = newNode;
    }

    // Update appropriate count
    if (ticketType == 1) curr->vipCount++;
    else if (ticketType == 2) curr->earlyBirdCount++;
    else if (ticketType == 3) curr->generalCount++;
}

//validate availability of the specific ticket type
bool TicketSystem::isTicketAvailable(const string& matchDate, int ticketType) 
{
    MatchTickets* curr = ticketCounts;

    while (curr && curr->matchDate != matchDate) 
    {
        curr = curr->next;
    }

    if (!curr) 
    {
        // No tickets sold for this date yet, so all tickets are available
        return true;
    }

    if (ticketType == 1) return curr->vipCount < maxVIP;
    else if (ticketType == 2) return curr->earlyBirdCount < maxEarlyBird;
    else if (ticketType == 3) return curr->generalCount < maxGeneral;

    return false;
}



// Buy Ticket
void TicketSystem::buyTicket() 
{
    string qualifierDate = MatchStage::getMatchDate(MatchStage::QUALIFIER);
    string rrDate = MatchStage::getMatchDate(MatchStage::ROUND_ROBIN_1);
    string semiDate = MatchStage::getMatchDate(MatchStage::SEMI_FINAL);
    string finalDate = MatchStage::getMatchDate(MatchStage::FINAL);
    
    try 
    {
        // Create an array of available matches (today and future)
        string stageDates[4] = { qualifierDate, rrDate, semiDate, finalDate };
        string stageNames[4] = { "Qualifier", "Round Robin", "Semi-Final", "Final" };
        bool isMatchToday[4] = { false }; // Track which matches are today
        int displayCount = 0;

        cout << "Available Matches:\n";

        // Check and display the four specific match stages
        int validChoices = 0;
        for (int i = 0; i < 4; ++i) 
        {
            // Skip if date is not valid
            if (stageDates[i].empty()) 
            {
                continue;
            }

            // Check if the match is today or in future
            bool matchIsToday = isToday(stageDates[i]);
            bool matchIsFuture = isFutureDate(stageDates[i]);

            if (matchIsToday || matchIsFuture) 
            {
                validChoices++;
                cout << validChoices << ". " << stageDates[i];

                if (matchIsToday) 
                {
                    cout << " (TODAY)";
                    isMatchToday[i] = true;
                }

                cout << " - " << stageNames[i] << " Stage\n";
            }
        }

        //validate available matches
        if (validChoices == 0) 
        {
            cout << "No available matches for today or future dates!\n";
            return;
        }

        int matchChoice;
        cout << "\nSelect Match (Enter number): ";
        cin >> matchChoice;

        // Validate input is integer
        if (!checkInt() || matchChoice < 1 || matchChoice > validChoices) 
        {
            cout << "Invalid match selection!\n";
            return;
        }

        // Find the actual selected date based on choice
        int validIndex = 0;
        int selectedIndex = -1;

        for (int i = 0; i < 4; ++i) 
        {
            if (stageDates[i].empty()) 
            {
                continue;
            }

            bool matchIsToday = isToday(stageDates[i]);
            bool matchIsFuture = isFutureDate(stageDates[i]);

            if (matchIsToday || matchIsFuture) 
            {
                validIndex++;
                if (validIndex == matchChoice) 
                {
                    selectedIndex = i;
                    break;
                }
            }
        }

        if (selectedIndex == -1) 
        {
            cout << "Error selecting match!\n";
            return;
        }

        string matchDate = stageDates[selectedIndex];

        string purchaseDate;
        cout << "Enter purchase date (YYYY-MM-DD): ";
        cin >> purchaseDate;

        // Validate purchase date format
        if (purchaseDate.length() != 10 || purchaseDate[4] != '-' || purchaseDate[7] != '-') {
            cout << "Invalid date format! Use YYYY-MM-DD format.\n";
            return;
        }

        // Determine available ticket types based on purchase date and match date
        bool isPurchaseDateBeforeMatch = false;
        bool isPurchaseDateOnMatch = false;
        bool isPurchaseDateAfterMatch = false;

        // Compare purchase date with match date
        if (purchaseDate < matchDate) {
            isPurchaseDateBeforeMatch = true;
        }
        else if (purchaseDate == matchDate) {
            isPurchaseDateOnMatch = true;
        }
        else {
            isPurchaseDateAfterMatch = true;
        }

        // Display available ticket types based on purchase date
        cout << "Select Ticket Type:\n";

        if (isPurchaseDateAfterMatch) {
            cout << "Sorry, tickets are not available after the match.\n";
            return;
        }
        else if (isPurchaseDateOnMatch) {
            // On match day - only General tickets
            cout << "3. General\n";
        }
        else {
            // Before match - offer VIP and Early Bird
            cout << "1. VIP\n2. Early Bird\n";
        }


        string customerName;
        int ticketType;

        cout << "Enter Customer Name: ";
        cin.ignore(); // Clear any leftover input
        getline(cin, customerName); // Use getline to handle full names with spaces

        cout << "Enter ticket type: ";
        cin >> ticketType;

        // Validate ticket choice input 
        if (!checkInt()) 
        {
            cout << "Invalid ticket type!\n";
            return;
        }

        //Validate whether purchase date is today or future and sell respective ticket type
        if (isPurchaseDateOnMatch)
        {
            // Match day - only General tickets allowed
            if (ticketType != 3)
            {
                cout << "Error: Only General tickets (type 3) are available on the match day.\n";
                return;
            }
        }
        else if (isPurchaseDateBeforeMatch)
        {
            // Before match - only VIP or Early Bird tickets allowed
            if (ticketType != 1 && ticketType != 2)
            {
                cout << "Error: Only VIP (type 1) or Early Bird (type 2) tickets are available before match day.\n";
                return;
            }
        }

        // Check ticket limits
        if (!isTicketAvailable(matchDate, ticketType))
        {
            cout << "Sorry, this ticket type is sold out for the selected match date!\n";
            return;
        }

        string customerID = generateCustomerID();

        Ticket* newTicket = new Ticket{ matchDate, customerID, customerName, ticketType, nullptr };

        // Add to the linked list in sorted order
        insertTicketSorted(newTicket);

        // Update count after purchase
        updateTicketCount(matchDate, ticketType);

        // Save to file
        ofstream file("Ticket.csv", ios::app);
        if (!file) 
        {
            cout << "Error opening Ticket.csv for writing!\n";
            delete newTicket; // Clean up if we can't save
            return;
        }

        file << matchDate << "," << customerID << "," << customerName << "," << ticketType << "\n";
        file.close();

        cout << "Ticket purchased successfully! Customer ID: " << customerID << "\n";
        cout << "Match: " << stageNames[selectedIndex] << " on " << matchDate << "\n";
    }
    catch (const std::exception& e) 
    {
        cout << "Error in buyTicket: " << e.what() << endl;
    }
    catch (...) 
    {
        cout << "Unknown error in buyTicket" << endl;
    }
}


//Load all ticket data
void TicketSystem::loadTicketData() 
{
    //Read ticket.csv
    ifstream file("Ticket.csv");
    if (!file) 
    {
        cout << "Error: Could not open Ticket.csv!\n";
        return;
    }

    string line;
    getline(file, line); // Skip header if present

    // Clear existing ticket counts
    while (ticketCounts) 
    {
        MatchTickets* temp = ticketCounts;
        ticketCounts = ticketCounts->next;
        delete temp;
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        string matchDate, customerID, customerName, ticketTypeStr;

        getline(ss, matchDate, ',');
        getline(ss, customerID, ',');
        getline(ss, customerName, ',');
        getline(ss, ticketTypeStr, ',');

        int ticketType = stoi(ticketTypeStr);

        // Count tickets sold per type
        // Count tickets per date
        updateTicketCount(matchDate, ticketType);

        // Create and insert ticket
        Ticket* newTicket = new Ticket{ matchDate, customerID, customerName, ticketType, nullptr };
        insertTicketSorted(newTicket);

        cout << "Loaded ticket: " << matchDate << ", " << customerID << ", "
            << customerName << ", " << ticketType << std::endl;
    }

    file.close();
    cout << "Ticket data loaded successfully!\n";
}


//Simulate entry and exit flow of spectators
void TicketSystem::simulateSpectatorFlow() 
{
    // If no tickets are loaded, load them first
    if (!head) 
    {
        loadTicketData();
    }

    // Check if we have any tickets at all
    if (!head) 
    {
        cout << "No tickets found. Please add tickets first.\n";
        return;
    }

    // Create a set of unique dates from tickets
    string uniqueDates[100]; // Assuming max 100 unique dates
    int dateCount = 0;

    // Extract unique dates from tickets
    Ticket* current = head;
    while (current) 
    {
        // Check if this date is already in our array
        bool dateExists = false;
        for (int i = 0; i < dateCount; i++) 
        {
            if (uniqueDates[i] == current->matchDate) 
            {
                dateExists = true;
                break;
            }
        }

        // If date not found, add it
        if (!dateExists) 
        {
            uniqueDates[dateCount++] = current->matchDate;
        }

        current = current->next;
    }

    // Display available dates
    cout << "Available dates for simulation:\n";
    for (int i = 0; i < dateCount; i++)
    {
        cout << i + 1 << ". " << uniqueDates[i] << "\n";
    }

    // Get user choice
    int dateChoice;
    cout << "Select date to simulate (Enter number): ";
    cin >> dateChoice;

    // Validate input
    if (!checkInt() || dateChoice < 1 || dateChoice > dateCount) 
    {
        cout << "Invalid date selection!\n";
        return;
    }

    string selectedDate = uniqueDates[dateChoice - 1];
    cout << "Simulating spectator flow for date: " << selectedDate << "\n";

    // Store all spectators for the selected date
    struct Spectator 
    {
        string customerID;
        string name;
        int priority; // Lower number = higher priority
        Spectator* next;
    };

    // Create a copy of the spectator data for exit flow simulation
    struct SpectatorData 
    {
        string customerID;
        string name;
        int priority;
    };

    // Create arrays to store spectator data
    SpectatorData spectators[1000]; // Assuming max 1000 spectators
    int spectatorCount = 0;

    // Copy only tickets matching selected date to the array
    current = head;
    while (current) 
    {
        // Only process tickets matching the selected date
        if (current->matchDate == selectedDate) 
        {
            int priority;
            if (current->ticketType == 1) priority = 1;      // VIP
            else if (current->ticketType == 2) priority = 2; // Early Bird
            else priority = 3;                               // General

            // Store spectator data
            spectators[spectatorCount].customerID = current->customerID;
            spectators[spectatorCount].name = current->customerName;
            spectators[spectatorCount].priority = priority;
            spectatorCount++;
        }

        current = current->next;
    }

    if (spectatorCount == 0) {
        cout << "No tickets found for the selected date.\n";
        return;
    }

    //ENTRY FLOW SIMULATION
    cout << "\n--- ENTRY FLOW SIMULATION ---\n";
    cout << "Starting simulation for " << spectatorCount << " spectators...\n\n";

    // Build priority queue for entry (VIPs first)
    Spectator* entryHead = nullptr;

    // Copy spectators to priority queue for entry flow
    for (int i = 0; i < spectatorCount; i++) 
    {
        // Create new spectator node
        Spectator* newNode = new Spectator
        {
            spectators[i].customerID,
            spectators[i].name,
            spectators[i].priority,
            nullptr
        };

        // Insert based on priority
        if (!entryHead || spectators[i].priority < entryHead->priority) 
        {
            newNode->next = entryHead;
            entryHead = newNode;
        }
        else 
        {
            Spectator* curr = entryHead;
            while (curr->next && curr->next->priority <= spectators[i].priority) 
            {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
    }

    // Print header for the entry table
    cout << left << setw(10) << "Order" << setw(15) << "Customer ID"
        << setw(20) << "Name" << setw(15) << "Ticket Type" << "\n";
    cout << string(60, '-') << "\n";

    // Simulate entry flow
    int order = 1;
    Spectator* current_spectator = entryHead;
    while (current_spectator) 
    {
        string ticketType;
        if (current_spectator->priority == 1) ticketType = "VIP";
        else if (current_spectator->priority == 2) ticketType = "Early Bird";
        else ticketType = "General";

        cout << left << setw(10) << order
            << setw(15) << current_spectator->customerID
            << setw(20) << current_spectator->name
            << setw(15) << ticketType << "\n";

        current_spectator = current_spectator->next;
        order++;
    }

    cout << "\nAll spectators have entered successfully.\n";

    // Clean up entry queue
    while (entryHead) 
    {
        Spectator* temp = entryHead;
        entryHead = entryHead->next;
        delete temp;
    }

    //EXIT FLOW SIMULATION
    cout << "\n--- EXIT FLOW SIMULATION ---\n";
    cout << "Starting exit simulation (VIPs exit first)...\n\n";

    // Build priority queue for exit (VIPs first, same as entry)
    Spectator* exitHead = nullptr;

    // Copy spectators to priority queue for exit flow (same priority as entry)
    for (int i = 0; i < spectatorCount; i++) 
    {
        // Create new spectator node
        Spectator* newNode = new Spectator
        {
            spectators[i].customerID,
            spectators[i].name,
            spectators[i].priority,
            nullptr
        };

        // Insert based on priority - same as entry flow
        if (!exitHead || spectators[i].priority < exitHead->priority) 
        {
            newNode->next = exitHead;
            exitHead = newNode;
        }
        else 
        {
            Spectator* curr = exitHead;
            while (curr->next && curr->next->priority <= spectators[i].priority) 
            {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
    }

    // Print header for the exit table
    cout << left << setw(10) << "Order" << setw(15) << "Customer ID"
        << setw(20) << "Name" << setw(15) << "Ticket Type" << "\n";
    cout << string(60, '-') << "\n";

    // Simulate exit flow
    order = 1;
    current_spectator = exitHead;
    while (current_spectator) 
    {
        string ticketType;
        if (current_spectator->priority == 1) ticketType = "VIP";
        else if (current_spectator->priority == 2) ticketType = "Early Bird";
        else ticketType = "General";

        cout << left << setw(10) << order
            << setw(15) << current_spectator->customerID
            << setw(20) << current_spectator->name
            << setw(15) << ticketType << "\n";

        current_spectator = current_spectator->next;
        order++;
    }

    cout << "\nAll spectators have exited successfully.\n";

    // Clean up exit queue
    while (exitHead) 
    {
        Spectator* temp = exitHead;
        exitHead = exitHead->next;
        delete temp;
    }
}






