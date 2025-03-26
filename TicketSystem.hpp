#ifndef TICKETSYSTEM_HPP
#define TICKETSYSTEM_HPP

#include "getDate.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct MatchTickets 
{
    string matchDate;
    int vipCount;
    int earlyBirdCount;
    int generalCount;
    MatchTickets* next;
};

struct Ticket 
{
    string matchDate;
    string customerID;
    string customerName;
    int ticketType; // 1 = VIP, 2 = Early Bird, 3 = General
    Ticket* next;
};

class TicketSystem
{
private:
    // Example limit for demo only
    int maxVIP = 3;      
    int maxEarlyBird = 5;
    int maxGeneral = 5;

    MatchTickets* ticketCounts; // Head of linked list for ticket counts by date

    Ticket* head;
    string matchDates[100]; // Store match dates
    int matchCount;
    int lastCustomerID;

    bool isDateTodayOrFuture(const string& date);
    bool isToday(const string& date);
    bool isFutureDate(const string& date);

    string generateCustomerID();
    void insertTicketSorted(Ticket* newTicket);

public:
    TicketSystem();
    ~TicketSystem();

    bool checkInt();
    void updateTicketCount(const string& matchDate, int ticketType);
    bool isTicketAvailable(const string& matchDate, int ticketType);
    void buyTicket();
    void loadTicketData();
    void simulateSpectatorFlow();
};


#endif
