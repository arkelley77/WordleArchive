#include "gui_main.h"

using std::cerr;
using std::endl;
using std::string;

class WordlePlayer {
public:
    WordlePlayer(string cwd)
        : cwd(cwd),
        word_list(), dictionary(), custom_dictionary(), custom_word_list(),
        stats(), current_word(""), valid_guesses(), keys(),
        game_mode(todays_word), active(loading),
        gui_cfg(),
        window(), font(), color_mode(lightMode()),
        buttons(), texts() {
        if (!font.loadFromFile(cwd + "/files/Mona-Sans-Regular.ttf")) {
          cerr << "Failed to load font file" << endl;
          close(1);
        }
        try {
            //cerr << gui_cfg.width << ' ' << gui_cfg.height << endl;
            gui_cfg.readFromFile(cwd + "/files/gui_cfg.txt");
            //cerr << gui_cfg.width << ' ' << gui_cfg.height << endl;
        }
        catch (...) {
            cerr << "Failed to read graphics configuration file" << endl;
            close(1);
        }
        window.create(gui_cfg.getVideoMode(), "Wordle Archive");
        color_mode = (gui_cfg.dark_mode) ? darkMode() : lightMode();
        resize(window.getView().getViewport());
        initializeUI();
        draw();
        try {
            dictionary.readFromFile(cwd + "/files/word_list.txt");
        }
        catch (...) {
            cerr << "Failed to read dictionary file" << endl;
            //throw std::runtime_error("failed reading dictionary file");
            close(1);
        }
        try {
            word_list.readFromFile(cwd + "/files/wordles.txt");
            dictionary.combine(word_list);
        }
        catch (...) {
            cerr << "Failed to read word list file" << endl;
            //throw std::runtime_error("failed reading word list file");
            exit(1);
        }
        try {
            custom_word_list.readFromFile(cwd + "/custom_word_list.txt");
            custom_dictionary.readFromFile(cwd + "/custom_dictionary.txt");
            custom_dictionary.combine(custom_word_list);  // just in case they don't include the actual words
        }
        catch (const std::runtime_error&) {
            cerr << "Failed to read custom word list files" << endl;
            //throw std::runtime_error("failed reading custom word list file");
            custom_word_list.clear();
            custom_dictionary.clear();
        }
        try {
            stats.readFromFile(cwd + "/statistics.csv");
        }
        catch (const std::runtime_error& e) {
            cerr << ' ' << e.what() << ", Failed!" << endl;
            //throw std::runtime_error("failed reading statistics file");
            exit(1);
        }
        current_word.number = -1;
        active = main_menu;
    }

    //void promptMenu() {
    //    if (stats.numUnfinishedGames() != 0) {
    //    promptUnfinishedGames:
    //        cout << "Found " << stats.numUnfinishedGames() << " unfinished game";
    //        if (stats.numUnfinishedGames() > 1) cout << 's';
    //        cout << ". Would you like to play ";
    //        if (stats.numUnfinishedGames() > 1) cout << "them";
    //        else cout << "it";
    //        cout << "? (Y/N) " << flush;
    //        char response;
    //        cin >> response;
    //        switch (response) {
    //        case 'y':
    //        case 'Y': current_operation = resume_game; return;
    //        case 'n':
    //        case 'N': break;
    //        default: goto promptUnfinishedGames;
    //        }
    //    }
    //    cout << "Menu :\n";
    //    cout << "0: Close program:      type 'q', 'quit', or '0', or\n";
    //    cout << "                       press " << EOF_KEY << " at any time\n";
    //    cout << "1: Word by number:     type 'number' or '1'\n";
    //    cout << "2: Word by date:       type 'date' or '2'\n";
    //    cout << "3: Get today's Wordle: type 'today' or '3'\n";
    //    cout << "4: Random Wordle Word: type 'random' or '4'\n";
    //    cout << "5: Random Hard Word:   type 'hard' or '5'\n";
    //    cout << "6: Info (HELP):        type 'info', 'help', or '6'\n";
    //    if (!custom_word_list.empty()) {
    //        cout << "7: Custom Wordle:      type 'custom' or '7'\n";
    //    }
    //    cout << std::flush;
    //    string response;
    //    cin >> response;
    //    response = strToUpper(response);
    //    if (response == "Q" || response == "QUIT" || response == "0" || cin.eof()) {
    //        current_operation = quit;
    //    }
    //    else if (response == "NUMBER" || response == "1") {
    //        current_operation = word_by_number;
    //    }
    //    else if (response == "DATE" || response == "2") {
    //        current_operation = word_by_date;
    //    }
    //    else if (response == "TODAY" || response == "3") {
    //        current_operation = todays_word;
    //    }
    //    else if (response == "RANDOM" || response == "4") {
    //        current_operation = random_wordle_word;
    //    }
    //    else if (response == "HARD" || response == "5") {
    //        current_operation = random_dict_word;
    //    }
    //    else if (response == "INFO" || response == "HELP" || response == "6") {
    //        promptInfo();
    //    }
    //    else if (!custom_word_list.empty() && (response == "CUSTOM" || response == "7")) {
    //        current_operation = custom_word;
    //    }
    //    else {
    //        cout << Color::error << "Invalid menu response, please try again." << Color::defaults << endl;
    //        cin.clear();
    //        if (cin.fail()) cin.ignore();
    //        promptMenu();
    //    }
    //}

    //void promptInfo() {
    //    cout << "\n\nWordle Archive Reborn Help\n";

    //    cout << "\n0: Close program:      type 'q', 'quit', or '0', or\n";
    //    cout << "                       press " << EOF_KEY << " at any time\n";
    //    cout << "    Exits the program.\n";

    //    cout << "\n1: Word by number:     type 'number' or '1'\n";
    //    cout << "    Selects a word by its word number.\n";
    //    cout << "    For example, today's Wordle is Wordle #";
    //    cout << word_list.getWordByDate(todaysDate()).number << ".\n";

    //    cout << "\n2: Word by date:       type 'date' or '2'\n";
    //    cout << "    Selects a word given the month, day, and year.\n";
    //    cout << "    Yes, you can play a Wordle from the future.\n";

    //    cout << "\n3: Get today's Wordle: type 'today' or '3'\n";
    //    cout << "    Allows you to play the Wordle of the day.\n";
    //    cout << "    This is the same one that the New York Times has.\n";

    //    cout << "\n4: Random Wordle Word: type 'random' or '4'\n";
    //    cout << "    Selects a Wordle at random from the New York Times\n";
    //    cout << "    list of correct Wordle answers.\n";

    //    cout << "\n5: Random Hard Word:   type 'hard' or '5'\n";
    //    cout << "    Selects a Wordle at random from the New York Times\n";
    //    cout << "    dictionary of possible Wordle guesses. This mode is\n";
    //    cout << "    substantially harder due to the inclusion of words\n";
    //    cout << "    like 'speug', but allows for greater variety due to\n";
    //    cout << "    drawing from a list of over 12,000 words.\n";

    //    cout << "\n6: Info (HELP):        type 'info', 'help', or '6'\n";
    //    cout << "    Displays this screen.\n";

    //    cout << "\n7: Custom Wordle:      type 'custom' or '7'\n";
    //    cout << "    This program has the ability to use words of an arbitrary\n";
    //    cout << "    length using a custom word list. Simply add a collection\n";
    //    cout << "    of words to the file 'custom_word_list.txt', and this\n";
    //    cout << "    option will appear. When selected, the program will select\n";
    //    cout << "    one of these custom words at random and give you 6 guesses.\n";

    //    cout << "\nPress enter to return to the menu.\n";
    //    while (cin.get() != '\n') {
    //        if (cin.eof()) {
    //            current_operation = quit;
    //            return;
    //        }
    //    };
    //    promptMenu();
    //}

    //int promptWordNumber() {
    //    cout << "Please select a Wordle by number: " << flush;
    //    int word_number;
    //    cin >> word_number;
    //    if (cin.eof()) {
    //        current_operation = quit;
    //        return -1;
    //    }
    //    else if (word_number < 0 || word_number > word_list.size() - 1 || !cin.good()) {
    //        cout << "Invalid number, please try again." << endl;
    //        if (!cin.good()) {
    //            cin.clear();
    //            string throwaway;
    //            cin >> throwaway;
    //        }
    //        return promptWordNumber();
    //    }
    //    else {
    //        return word_number;
    //    }
    //}

    //string promptMonth() {
    //    cout << "Month: " << flush;
    //    string response;
    //    cin >> response;
    //    if (cin.eof()) {
    //        current_operation = quit;
    //        return "";
    //    }
    //    else if (!cin.good() || response.empty() || monthToInt(response) == -1) {
    //        cout << Color::error << "Invalid response, please try again." << Color::defaults << endl;
    //        if (!cin.good()) {
    //            cin.clear();
    //            string throwaway;
    //            cin >> throwaway;
    //        }
    //        return promptMonth();
    //    }
    //    else {
    //        return response;
    //    }
    //}

    //int promptDay(int month) {
    //    cout << "Day: " << flush;
    //    int response;
    //    cin >> response;
    //    if (cin.eof()) {
    //        current_operation = quit;
    //        return -1;
    //    }
    //    else if (!cin.good() || response <= 0 || response > daysInMonth(month)) {
    //        cout << Color::error << "Invalid day, please try again." << Color::defaults << endl;
    //        if (!cin.good()) {
    //            cin.clear();
    //            string throwaway;
    //            cin >> throwaway;
    //        }
    //        return promptDay(month);
    //    }
    //    else {
    //        return response;
    //    }
    //}

    //int promptYear() {
    //    cout << "Year: " << flush;
    //    int response;
    //    cin >> response;
    //    if (cin.eof()) {
    //        current_operation = quit;
    //        return -1;
    //    }
    //    else if (!cin.good()) {
    //        cout << Color::error << "Invalid year, please try again." << Color::defaults << endl;
    //        if (!cin.good()) {
    //            cin.clear();
    //            string throwaway;
    //            cin >> throwaway;
    //        }
    //        return promptYear();
    //    }
    //    else if (2021 <= response && response <= 2027) {
    //        return response;
    //    }
    //    else if (21 <= response && response <= 27) {
    //        return response + 2000;
    //    }
    //    else {
    //        cout << Color::error << "Invalid year, please try again." << Color::defaults << endl;
    //        if (!cin.good()) {
    //            cin.clear();
    //            string throwaway;
    //            cin >> throwaway;
    //        }
    //        return promptYear();
    //    }
    //}

    //int promptWordDate() {
    //    cout << "\nPlease enter the date. You may use any format.\n";
    //    string _ = promptMonth();
    //    if (_.empty()) return -1;
    //    int month = monthToInt(_);
    //    int day = promptDay(month);
    //    if (day == -1) return -1;
    //    int year = promptYear();
    //    if (year == -1) return -1;
    //    if (month == 1 && day == 28 && year % 4 != 0) {
    //        cout << Color::error << "Not a leap year" << Color::defaults << endl;
    //        if (!cin.good()) {
    //            cin.clear();
    //            string throwaway;
    //            cin >> throwaway;
    //        }
    //        return promptWordDate();
    //    }
    //    int date = Date(month, day, year);
    //    try {
    //        word_list.getWordByDate(date);
    //        return date;
    //    }
    //    catch (const std::exception& e) {
    //        cout << Color::error << "There is no Wordle for this date." << Color::defaults << endl;
    //        if (!cin.good()) {
    //            cin.clear();
    //            string throwaway;
    //            cin >> throwaway;
    //        }
    //        return promptWordDate();
    //    }
    //}

    //void prepKeyboard() {
    //    for (auto it : keyboard) {
    //        if (it != '\n' && it != ' ') {
    //            if (keys.find(it) != keys.end()) keys.erase(it);
    //            keys.emplace(it, Color::grey);
    //        }
    //    }
    //}

    //void printPrevGuesses() {
    //    prepKeyboard();

    //    for (auto& guess : valid_guesses) {
    //        for (string::size_type i = 0; i < (59 - current_word.word.size()) / 2; ++i) {
    //            cout << Color::defaults << " ";
    //        }
    //        for (string::size_type i = 0; i < current_word.word.size(); ++i) {
    //            if (guess[i] == current_word.word[i]) {
    //                // right letter in the right spot, highest priority
    //                cout << Color::correct << guess[i] << Color::defaults;
    //                keys[guess[i]] = Color::correct;
    //            }
    //            else if (current_word.word.find(guess[i]) == string::npos) {
    //                // wrong letter entirely
    //                cout << Color::wrong << guess[i] << Color::defaults;
    //                if (keys[guess[i]] == Color::grey) keys[guess[i]] = Color::wrong;
    //            }
    //            else {
    //                // right letter, wrong place. this *should* be easy, buuut...
    //                // there's the issue of repeating letters. in both the guess & the word.

    //                // the number of times we've guessed this letter so far
    //                int num_repeats_in_guess_so_far = 0;
    //                string::size_type char_prev_loc = guess.find(guess[i]);
    //                while (char_prev_loc < i) {
    //                    ++num_repeats_in_guess_so_far;
    //                    if (current_word.word[char_prev_loc] == guess[i]) {
    //                        // this represents a past time where we got it right,
    //                        // so we shouldn't factor it in when counting repeats.
    //                        --num_repeats_in_guess_so_far;
    //                    }
    //                    try {
    //                        char_prev_loc = guess.find(guess[i], char_prev_loc + 1);
    //                    }
    //                    catch (const std::exception& e) {
    //                        // this should only happen if char_prev_loc is the last index
    //                        break;
    //                    }
    //                }
    //                // the number of times this letter appears in the word
    //                int num_repeats_in_word = 0;
    //                char_prev_loc = current_word.word.find(guess[i]);
    //                while (char_prev_loc != string::npos) {
    //                    ++num_repeats_in_word;
    //                    if (guess[char_prev_loc] == guess[i]) {
    //                        // this represents a future time where we got it right,
    //                        // so we shouldn't factor it in when counting repeats.
    //                        --num_repeats_in_word;
    //                    }
    //                    try {
    //                        char_prev_loc = current_word.word.find(guess[i], char_prev_loc + 1);
    //                    }
    //                    catch (const std::exception& e) {
    //                        // this should only happen if char_prev_loc is the last index
    //                        break;
    //                    }
    //                }

    //                // if the letter appears more times in the word than we've guessed so
    //                // far, then this iteration of the letter should be highlighted.
    //                if (num_repeats_in_word > num_repeats_in_guess_so_far) {
    //                    cout << Color::wrong_place << guess[i] << Color::defaults;
    //                    if (keys[guess[i]] != Color::correct) keys[guess[i]] = Color::wrong_place;
    //                }
    //                else {
    //                    cout << Color::wrong << guess[i] << Color::defaults;
    //                    if (keys[guess[i]] == Color::grey) keys[guess[i]] = Color::wrong;
    //                }
    //            }
    //        }
    //        cout << Color::defaults << '\n';
    //    }
    //    for (string::size_type i = 0; i < 6 - valid_guesses.size(); ++i) {
    //        cout << '\n';
    //    }
    //    cout << flush;
    //}

    //void printStatistics() {
    //    cout << Color::defaults << "Statistics:\n";
    //    cout << "Win rate: " << (int)(stats.calcWinRate() * 100) << "%\n";
    //    cout << "Games played: " << stats.numFinishedGames() << '\n';
    //    cout << "Average number of guesses: " << stats.calcAverageNumGuesses() << '\n';
    //    cout << '\n';
    //    for (int i = 1; i < 7; ++i) {
    //        cout << i << " |";
    //        for (int j = 0; j < (double)stats.frequencyLookup(i) * 59.0 / (double)stats.numFinishedGames(); ++j) {
    //            cout << '#';
    //        }
    //        cout << ' ' << stats.frequencyLookup(i) << '\n';
    //    }
    //    cout << endl;
    //}

    //void printKeyboard() {
    //    cout << Color::defaults << "                        ";
    //    for (auto it : keyboard) {
    //        if (it == '\n') {
    //            cout << Color::defaults << it << "                        ";
    //        }
    //        else if (it == ' ') {
    //            cout << Color::defaults << it;
    //        }
    //        else {
    //            cout << keys[it] << it << Color::defaults;
    //        }
    //    }
    //    cout << endl;
    //}

    //MenuOption promptNextWordle() {
    //    printStatistics();
    //    if (current_operation == resume_game && stats.numUnfinishedGames() == 0) {
    //        return home;
    //    }
    //    if (current_word.number == -1) {
    //        // custom wordle
    //        return home;
    //    }
    //    cout << "Would you like to play the next one?\n";
    //    cout << "Type 'y' or 'yes' to go to the next Wordle.\n";
    //    cout << "Type 'n' or 'no' to go back to the menu.\n";
    //    cout << "Type 'q' or 'quit' to exit.\n";
    //    cout << "Any other input will send you back to the main menu.\n";
    //    cout << ">> " << flush;
    //    string response;
    //    cin >> response;
    //    response = strToUpper(response);
    //    if (response == "Y" || response == "YES") {
    //        if (current_operation == random_wordle_word) return random_wordle_word;
    //        else if (current_operation == random_dict_word) return random_dict_word;
    //        else if (current_operation == resume_game) return resume_game;
    //        else return next_word;
    //    }
    //    else if (response == "N" || response == "NO") {
    //        return home;
    //    }
    //    else if (response == "Q" || response == "QUIT") {
    //        return quit;
    //    }
    //    else return home;
    //}

    //void playWordle() {
    //    WordleGame game = stats.loadEntry(current_word);
    //    if (current_operation != resume_game) {
    //        switch (game.state) {
    //            char response;
    //        case won:
    //            if (current_operation == random_wordle_word || current_operation == random_dict_word) {
    //                return;
    //            }
    //            cout << "You've already gotten this Wordle correct ";
    //            if (game.guesses.size() == 1) cout << "on the first try.\n";
    //            else cout << "in " << game.guesses.size() << " tries.\n";
    //            cout << "Are you sure you'd like to restart it? (Y/N) " << flush;
    //            while (true) {
    //                cin >> response;
    //                if (toupper(response) == 'Y') {
    //                    game.guesses.clear();
    //                    break;
    //                }
    //                else if (toupper(response) == 'N') {
    //                    current_operation = promptNextWordle();
    //                    return;
    //                }
    //                else {
    //                    cout << "Invalid response. Please type either 'Y' or 'N'. " << flush;
    //                }
    //            }
    //            break;
    //        case lost:
    //            if (current_operation == random_wordle_word || current_operation == random_dict_word) {
    //                return;
    //            }
    //            cout << "You've already played this Wordle and failed.\n";
    //            cout << "Are you sure you'd like to restart it? (Y/N) " << flush;
    //            while (true) {
    //                cin >> response;
    //                if (toupper(response) == 'Y') {
    //                    game.guesses.clear();
    //                    break;
    //                }
    //                else if (toupper(response) == 'N') {
    //                    current_operation = promptNextWordle();
    //                    return;
    //                }
    //                else {
    //                    cout << "Invalid response. Please type either 'Y' or 'N'. " << flush;
    //                }
    //            }
    //            break;
    //        case unfinished:
    //            if (game.guesses.empty()) break;
    //            cout << "Save data found for this Wordle.";
    //            cout << "Would you like to load your previous guesses? (Y/N) " << flush;
    //            while (true) {
    //                cin >> response;
    //                if (toupper(response) == 'Y') break;
    //                else if (toupper(response) == 'N') {
    //                    current_operation = promptNextWordle();
    //                    return;
    //                }
    //                else {
    //                    cout << "Invalid response. Please type either 'Y' or 'N'. " << flush;
    //                }
    //            }
    //        }
    //    }

    //    int guesses_remaining = 6 - (int)game.guesses.size();
    //    valid_guesses = game.guesses;
    //    string guess;

    //    cout << "\n\n";
    //    string header = current_word.getHeader();
    //    for (string::size_type i = 0; i < (59 - header.size()) / 2; ++i) {
    //        cout << ' ';
    //    }
    //    cout << header;
    //    cout << "\n";
    //    prepKeyboard();
    //    cout << '\n';
    //    printPrevGuesses();
    //    cout << '\n';
    //    printKeyboard();
    //    cout << '\n';
    //    for (string::size_type i = 0; i < (59 - current_word.word.size()) / 2 - 3; ++i) {
    //        cout << ' ';
    //    }
    //    cout << ">> " << flush;
    //    cin >> guess;
    //    while (!cin.eof() && !cin.bad()) {
    //        if (guess.size() != current_word.word.size()) {
    //            cout << "\n\n";
    //            header = current_word.getHeader();
    //            for (string::size_type i = 0; i < (59 - header.size()) / 2; ++i) {
    //                cout << ' ';
    //            }
    //            cout << header;
    //            if (guesses_remaining > 1) {
    //                cout << "                        " << guesses_remaining << " tries left\n\n";
    //            }
    //            else {
    //                cout << "                        " << guesses_remaining << " try left\n\n";
    //            }
    //            cout << "                 " << Color::error << "Word not the right length" << Color::defaults;
    //            cout << '\n';
    //            printPrevGuesses();
    //            cout << '\n';
    //            printKeyboard();
    //            cout << '\n';
    //            for (string::size_type i = 0; i < (59 - current_word.word.size()) / 2 - 3; ++i) {
    //                cout << ' ';
    //            }
    //            cout << ">> " << flush;
    //            cin >> guess;
    //            continue;
    //        }
    //        if (cin.fail() || (!dictionary.isWordInList(guess) && (current_word.number != -1 || !custom_dictionary.isWordInList(guess)))) {
    //            cout << "\n\n";
    //            header = current_word.getHeader();
    //            for (string::size_type i = 0; i < (59 - header.size()) / 2; ++i) {
    //                cout << ' ';
    //            }
    //            cout << header;
    //            if (guesses_remaining > 1) {
    //                cout << "                        " << guesses_remaining << " tries left\n\n";
    //            }
    //            else {
    //                cout << "                        " << guesses_remaining << " try left\n\n";
    //            }
    //            cout << "                   " << Color::error << "Word not in word list" << Color::defaults;
    //            cout << '\n';
    //            printPrevGuesses();
    //            cout << '\n';
    //            printKeyboard();
    //            cout << '\n';
    //            for (string::size_type i = 0; i < (59 - current_word.word.size()) / 2 - 3; ++i) {
    //                cout << ' ';
    //            }
    //            cout << ">> " << flush;
    //            cin >> guess;
    //            continue;
    //        }
    //        --guesses_remaining;
    //        valid_guesses.push_back(strToUpper(guess));
    //        stats.addEntry(current_word, valid_guesses);
    //        if (strToUpper(guess) == current_word.word) {
    //            cout << "\n\n";
    //            header = current_word.getHeader();
    //            for (string::size_type i = 0; i < (59 - header.size()) / 2; ++i) {
    //                cout << ' ';
    //            }
    //            cout << header;
    //            cout << "\n\n";
    //            printPrevGuesses();
    //            cout << "\nGreat job! You solved it in " << 6 - guesses_remaining;
    //            if (6 - guesses_remaining > 1) {
    //                cout << " tries.\n";
    //            }
    //            else {
    //                cout << " try.\n";
    //            }
    //            valid_guesses.clear();
    //            prepKeyboard();
    //            current_operation = promptNextWordle();
    //            return;
    //        }
    //        if (guesses_remaining == 0) {
    //            cout << "\n\n";
    //            header = current_word.getHeader();
    //            for (string::size_type i = 0; i < (59 - header.size()) / 2; ++i) {
    //                cout << ' ';
    //            }
    //            cout << header;
    //            cout << "                       " << Color::error << guesses_remaining << " tries left\n\n" << Color::defaults;
    //            printPrevGuesses();
    //            cout << "\n\nThat's too bad. The word was " << current_word.word << ".\n";
    //            valid_guesses.clear();
    //            prepKeyboard();
    //            current_operation = promptNextWordle();
    //            return;
    //        }
    //        cout << "\n\n";
    //        header = current_word.getHeader();
    //        for (string::size_type i = 0; i < (59 - header.size()) / 2; ++i) {
    //            cout << ' ';
    //        }
    //        cout << header;
    //        if (guesses_remaining == 1) {
    //            cout << "                       " << Color::error << guesses_remaining << " try left\n\n";
    //        }
    //        else {
    //            cout << "                       " << guesses_remaining << " tries left\n\n";
    //        }
    //        printPrevGuesses();
    //        cout << "\n\n";
    //        printKeyboard();
    //        cout << '\n';
    //        for (string::size_type i = 0; i < (59 - current_word.word.size()) / 2 - 3; ++i) {
    //            cout << ' ';
    //        }
    //        cout << ">> " << flush;
    //        cin >> guess;
    //    }
    //    current_operation = quit;
    //}

    void initializeUI() {
      buttons.clear();
      texts.clear();

      const sf::Vector2f& screen_center = window.getView().getCenter();

      // set up a base button that all buttons will use
      Button button(window);
      button.text.setColor(color_mode.outline);
      //button.text.setFillColor(sf::Color::Transparent);
      //button.text.setOutlineColor(sf::Color::Transparent);
      button.text.setFont(font);
      button.rect.setFillColor(color_mode.button);
      button.rect.setOutlineColor(color_mode.outline);
      button.rect.setOutlineThickness(outline_thickness);

      // and a base text for text items
      Text text(window);
      text.text.setColor(color_mode.outline);
      text.text.setFont(font);

      // Loading screen
      text.text.setCharacterSize(120);
      text.text.setString("Loading...");
      centerOrigin(text.text);
      text.text.setPosition(screen_center);
      texts.push_back(text);

      // Main menu
      text.text.setCharacterSize(150);
      text.text.setString("Wordle Archive");
      centerOrigin(text.text);
      text.text.setPosition(screen_center.x, screen_center.y - 75 - 48 - gap_size * 1.5f);
      texts.push_back(text);

      text.text.setCharacterSize(96);
      text.text.setString(L"by Alex Kelley");
      centerOrigin(text.text);
      text.text.setPosition(screen_center.x, screen_center.y - 48 - gap_size / 2.f);
      texts.push_back(text);

      button.text.setString("Play");
      button.text.setCharacterSize(96);
      button.rect.setSize({ text.text.getLocalBounds().width, 100 });
      centerOrigin(button.rect);
      button.rect.setPosition(screen_center.x, screen_center.y + 50 + gap_size / 2.f);
      button.on_click = [this]() { active = game_select; };
      buttons.push_back(button);

      button.rect.setPosition(screen_center.x, screen_center.y + 150 + gap_size * 1.5f);
      if (!gui_cfg.dark_mode) {
        button.text.setString("Dark Mode");
        button.on_click = [this]() {
          gui_cfg.dark_mode = false;
          color_mode = lightMode();
          initializeUI();
          UIElement::something_changed = true;
        };
      }
      else {
        button.text.setString("Light Mode");
        button.on_click = [this]() {
          gui_cfg.dark_mode = true;
          color_mode = darkMode();
          initializeUI();
          UIElement::something_changed = true;
        };
      }
      buttons.push_back(button);

      // Game select
      button.rect.setSize({ 1000, 100 });
      centerOrigin(button.rect);
      button.text.setString("Play Today's Word");
      button.rect.setPosition(screen_center.x, screen_center.y + 50 + gap_size / 2.f);
      button.on_click = [this]() {
        int date = todaysDate();
        try {
          current_word = word_list.getWordByDate(date);
        }
        catch (const std::exception& e) {
          //popUp("No Wordle Found!");
        }
        active = in_game;
      };
      buttons.push_back(button);

      button.text.setString("Select a Word");
      button.rect.setPosition(screen_center.x, screen_center.y + 150 + gap_size * 1.5f);
      button.on_click = [this]() { active = word_select; };
      buttons.push_back(button);

      button.text.setString("Random NYT Word");
      button.rect.setPosition(screen_center.x, screen_center.y + 250 + gap_size * 2.5f);
      button.on_click = [this]() {
        current_word = word_list.getRandomWord();
        active = in_game;
      };
      buttons.push_back(button);

      button.text.setString("Random Hard Word");
      button.rect.setPosition(screen_center.x, screen_center.y + 350 + gap_size * 3.5f);
      button.on_click = [this]() {
        current_word = dictionary.getRandomWord();
        active = in_game;
      };
      buttons.push_back(button);

      button.text.setString("Random Custom Word");
      button.rect.setPosition(screen_center.x, screen_center.y + 450 + gap_size * 4.5f);
      button.on_click = [this]() {
        current_word = custom_word_list.getRandomWord();
        active = in_game;
      };
      buttons.push_back(button);

    }

    void showLoadingScreen() {
      texts[loading_text].show();
    }

    void showMainMenu() {
      buttons[play_button].show();
      buttons[color_mode_button].show();
      texts[title_text].show();
      texts[subtitle_text].show();
    }

    void showGameSelect() {
      buttons[todays_word_button].show();
      buttons[select_word_button].show();
      buttons[random_nyt_word_button].show();
      buttons[random_hard_word_button].show();
      if (!custom_word_list.empty()) buttons[random_custom_word_button].show();
      texts[title_text].show();
      texts[subtitle_text].show();
    }
    
    void draw() {
      window.clear(color_mode.background);
      sf::RectangleShape rect;
      rect.setPosition(0, 0);
      rect.setSize({ view_width, view_height });
      rect.setFillColor(color_mode.background);
      rect.setOutlineColor(color_mode.outline);
      rect.setOutlineThickness(5.f);
      window.draw(rect);
      for (auto& it : buttons) it.hide();
      for (auto& it : texts) it.hide();
      switch (active) {
      case loading:
        showLoadingScreen();
        break;
      case main_menu:
        showMainMenu();
        break;
      case game_select:
        showGameSelect();
        break;
      default:
        break;
      }
      for (auto& it : buttons) it.draw();
      for (auto& it : texts) it.draw();
      window.display();
    }

    template <class Size>
    void resize(Size& event) {
      float new_aspect_ratio = (float)event.width / (float)event.height;
      float target_aspect_ratio = 1.f;
      if (new_aspect_ratio > target_aspect_ratio) {
        sf::Vector2f new_size = {
          view_width * new_aspect_ratio,
          view_height
        };
        window.setView(sf::View(sf::FloatRect(-(new_size.x - view_width) / 2.f, 0.f, new_size.x, new_size.y)));
      }
      else if (new_aspect_ratio < target_aspect_ratio) {
        sf::Vector2f new_size = {
          view_width,
          view_height / new_aspect_ratio
        };
        window.setView(sf::View(sf::FloatRect(0.f, -(new_size.y - view_height) / 2.f, new_size.x, new_size.y)));
      }
      else window.setView(sf::View(sf::FloatRect(0.f, 0.f, view_width, view_height)));
    }

    void close(int exit_code = 0) {
      if (window.isOpen()) window.close();
      if (stats.isValid()) stats.writeToFile();
      gui_cfg.writeToFile(cwd + "/files/gui_cfg.txt");
      exit(exit_code);
    }

    int mainloop() {
        
        sf::Clock clock;
        while (window.isOpen()) {
          sf::Event event;
          while (window.pollEvent(event)) {
            typedef sf::Event::EventType EventType;
            switch (event.type) {
            case EventType::Closed:
              close();
              break;
            case EventType::Resized:
              resize(event.size);
              draw();
              break;
            default:
              for (auto& it : buttons) it.handleEvent(event);
              for (auto& it : texts) it.handleEvent(event);
              break;
            }
            if (
              UIElement::something_changed// ||
              //clock.getElapsedTime() > sf::seconds(1.f / 60.f)
              ) {
              UIElement::something_changed = false;
              for (auto& it : buttons) it.onRefresh();
              for (auto& it : texts) it.onRefresh();
              draw();
              clock.restart();
            }
          }
          if (
            UIElement::something_changed// ||
            //clock.getElapsedTime() > sf::seconds(1.f / 60.f)
            ) {
            UIElement::something_changed = false;
            for (auto& it : buttons) it.onRefresh();
            for (auto& it : texts) it.onRefresh();
            draw();
            clock.restart();
          }
        }
        //cout << '\n';
        //while (current_operation != quit && !cin.eof()) {
        //    if (cin.bad()) {
        //        cout << Color::error << "Fatal error in console input" << Color::defaults << endl;
        //        return 1;
        //    }
        //    if (cin.eof()) {
        //        current_operation = quit;
        //    }
        //    int word_number = -1;
        //    int date = -1;
        //    WordleGame game;
        //    switch (current_operation) {
        //    case quit:
        //        stats.writeToFile();
        //        return 0;
        //    case word_by_number:
        //        word_number = promptWordNumber();
        //        if (word_number == -1) break;
        //        current_word = word_list.getWordByNumber(word_number);
        //        playWordle();
        //        break;
        //    case word_by_date:
        //        date = promptWordDate();
        //        if (date == -1) break;
        //        current_word = word_list.getWordByDate(date);
        //        playWordle();
        //        break;
        //    case todays_word:
        //        date = todaysDate();
        //        try {
        //            current_word = word_list.getWordByDate(date);
        //        }
        //        catch (const std::exception& e) {
        //            cout << Color::error << "No Wordle found" << Color::defaults << endl;
        //            current_operation = home;
        //            break;
        //        }
        //        playWordle();
        //        break;
        //    case random_wordle_word:
        //        current_word = word_list.getRandomWord();
        //        playWordle();
        //        break;
        //    case random_dict_word:
        //        current_word = dictionary.getRandomWord();
        //        playWordle();
        //        break;
        //    case custom_word:
        //        if (!custom_word_list.empty()) {
        //            current_word = custom_word_list.getRandomWord();
        //            playWordle();
        //            break;
        //        }
        //        else {
        //            cout << Color::error << "No custom Wordles" << Color::defaults << endl;
        //            current_operation = home;
        //            break;
        //        }
        //    case next_word:
        //        if (current_word.number == -1) {
        //            // custom word
        //            current_word = custom_word_list.getRandomWord();
        //            playWordle();
        //            break;
        //        }
        //        else if (current_word.number == word_list.size() - 1) {
        //            // there is no next word
        //            cout << "You've just played the last Wordle. Go play outside!" << endl;
        //            current_operation = home;
        //            break;
        //        }
        //        else {
        //            current_word = word_list.getWordByNumber(current_word.number + 1);
        //            playWordle();
        //            break;
        //        }
        //    case home:
        //        cout << '\n';
        //        promptMenu();
        //        break;
        //    case resume_game:
        //        game = stats.getUnfinishedGame();
        //        if (game.word_number >= 0) current_word = word_list.getWordByNumber(game.word_number);
        //        else if (game.word_number == -1) {
        //            current_word = WordleWord(-1, -1, -1, -1, game.word);
        //        }
        //        else if (game.word_number == -2) {
        //            current_word = WordleWord(-2, -2, -2, -2, game.word);
        //        }
        //        playWordle();
        //        break;
        //    }
        //}
        //stats.writeToFile();
        return 0;
    }

private:
    std::string cwd;

    WordList word_list;
    WordDict dictionary;
    CustomDict custom_dictionary;
    CustomWordList custom_word_list;
    WordleStatistics stats;
    WordleWord current_word;
    std::vector<string> valid_guesses;
    std::unordered_map<char, sf::Color> keys;

    GameMode game_mode;
    ActiveWindow active;

    GUIConfig gui_cfg;

    sf::RenderWindow window;
    sf::Font font;
    ColorMode color_mode;

    std::vector<Button> buttons;
    enum ButtonIDX : size_t {
      play_button = 0,
      color_mode_button = 1,
      todays_word_button = 2,
      select_word_button = 3,
      random_nyt_word_button = 4,
      random_hard_word_button = 5,
      random_custom_word_button = 6,
    };
    std::vector<Text> texts;
    enum TextIDX : size_t {
      loading_text = 0,
      title_text = 1,
      subtitle_text = 2,
    };
};

int main(int argc, char* argv[]) {
    if (argc == 0 || argv == nullptr) {
        cerr << "Error: No arguments" << endl;
        return 1;
    }
    string cwd(argv[0]);
    size_t slash_idx = cwd.size() - 1;
    for (; slash_idx != string::npos; --slash_idx) {
        if (cwd[slash_idx] == '/' || cwd[slash_idx] == '\\') {
            cwd = cwd.substr(0, slash_idx);
            break;
        }
    }
    if (slash_idx == ~(size_t)0) cwd = "";
    srand((unsigned int)time(NULL));
    WordlePlayer wordle_player(cwd);
    return wordle_player.mainloop();
}

