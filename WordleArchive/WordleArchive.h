#ifndef WORDLEARCHIVE_H
#define WORDLEARCHIVE_H

#include "color.h"
#include "word_list.h"
#include "word_dict.h"
#include "wordle_statistics.h"

#include <iostream>
#include <ctime>

#ifdef _WIN32
    // Windows
    #define EOF_KEY "Ctrl+Z then Enter"
#else  //
    #ifdef __APPLE__
        // Apple
        #define EOF_KEY "Ctrl+D (not ⌘)"
    #else  // not __APPLE__
        // Linux
        #define EOF_KEY "Ctrl+D"
    #endif // endif for __APPLE__
#endif // endif for _WIN32

enum MenuOption : char {
    quit, word_by_number, word_by_date, random_wordle_word,
    random_dict_word, custom_word, todays_word, next_word, home,
    resume_game, new_word_by_number, random_new_word,
};

const string keyboard = "QWERTYUIOP\n ASDFGHJKL \n  ZXCVBNM ";

string monthToString(int month) {
    switch (month) {
    case 0: return "Jan";
    case 1: return "Feb";
    case 2: return "Mar";
    case 3: return "Apr";
    case 4: return "May";
    case 5: return "Jun";
    case 6: return "Jul";
    case 7: return "Aug";
    case 8: return "Sep";
    case 9: return "Oct";
    case 10: return "Nov";
    case 11: return "Dec";
    }
    return "";
}

int todaysDate() {
    time_t now = time(NULL);
    tm* local_time = localtime(&now);
    return Date(local_time->tm_mon, local_time->tm_mday, local_time->tm_year + 1900);
}

int main(int argc, char* argv[]);

#endif
