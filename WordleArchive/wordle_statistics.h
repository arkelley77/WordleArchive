//
//  wordle_statistics.h
//  WordleArchive
//
//  Created by Alex Kelley on 5/14/22.
//

#ifndef wordle_statistics_h
#define wordle_statistics_h

#include "word_list.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using std::string;

enum GameState : char {
  won=0, lost=1, unfinished=2,
};

struct WordleGame {
  int word_number = -1;
  string word = "";
  std::vector<string> guesses = {};
  GameState state = unfinished;
};

class WordleStatistics {
  public:
    WordleStatistics() : filename(), games(), guess_frequencies({0, 0, 0, 0, 0, 0}), unfinished_games(), valid(true) {}

    void readFromFile(string filename);

    void writeToFile();

    void createFile();
  
    void addEntry(WordleWord word, std::vector<string> guesses);

    bool inFile(WordleWord word);

    std::vector<WordleGame>::size_type getIndexByWord(WordleWord word);

    WordleGame loadEntry(WordleWord word);

    size_t numFinishedGames();
    
    size_t numUnfinishedGames() { return unfinished_games.size(); }
  
    double calcWinRate();

    double calcAverageNumGuesses();

    inline int frequencyLookup(size_t num_guesses) {
      return guess_frequencies[num_guesses - 1];
    }

    bool isValid() const { return valid; }

    WordleGame getUnfinishedGame() {
      // sure it gets the games off the back of the pile, but who cares?
      WordleGame game = *(unfinished_games.back());
      unfinished_games.pop_back();
      return game;
    }
  private:
    string filename;
    std::unordered_map<string, WordleGame> games;
    std::array<int, 7> guess_frequencies;
    std::vector<WordleGame*> unfinished_games;
    bool valid;
};

#endif /* wordle_statistics_h */
