#ifndef GETDATE_HPP
#define GETDATE_HPP

#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

class MatchStage {
public:
    enum Stage { //Setting of the competition date
        QUALIFIER = 1, // Qualifier Date will be asign to current Date + 1 day
        ROUND_ROBIN_1 = 2,
        SEMI_FINAL = 3,
        FINAL = 4,
    };

    // Function to get the match date
    static string getMatchDate(Stage stage) {
        time_t now = time(0);
        tm ltm;
        tm* ltm_ptr = &ltm;

        localtime_s(ltm_ptr, &now);

        // add days based on the stage
        ltm_ptr->tm_mday += static_cast<int>(stage);
        mktime(ltm_ptr); // Normalize the date structure

        //return the date
        return to_string(1900 + ltm_ptr->tm_year) + "-" +
            (1 + ltm_ptr->tm_mon < 10 ? "0" : "") + to_string(1 + ltm_ptr->tm_mon) + "-" +
            (ltm_ptr->tm_mday < 10 ? "0" : "") + to_string(ltm_ptr->tm_mday);
    }

    //auto generate match id
    static string generateMatchID() {
        static int matchCounter = 1; // ensures it persists across calls
        string ss;
        if (matchCounter < 10) {
            ss = "M00" + to_string(matchCounter);
        }
        else if (matchCounter < 100) {
            ss = "M0" + to_string(matchCounter);
        }
        else {
            ss = "M" + to_string(matchCounter);
        }

        matchCounter++; //ensure match id is unique
        return ss;
    }
};


#endif
