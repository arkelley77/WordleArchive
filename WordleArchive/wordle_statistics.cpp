#include "wordle_statistics.h"

void WordleStatistics::readFromFile(string filename) {
  this->filename = filename;
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    // file does not exist so we need to make one
    createFile();
    return;
  }

  string line;
  // statistics
  getline(ifs, line); // reminder that the bar chart is only for 5-letter words
  getline(ifs, line); // stats header
  getline(ifs, line);
  while (!line.empty() && !ifs.eof()) {
    getline(ifs, line); // the stats list is only for the user so skip
  }
  if (ifs.eof()) return;  // we don't actually need the guess list
  // games
  getline(ifs, line); // "Completed Games"
  getline(ifs, line); // guess list header
  getline(ifs, line);
  while (!line.empty() && !ifs.eof()) {
    std::istringstream is;
    is.str(line);
    int word_number;
    string word;
    string state;
    string guess;
    getline(is, word, ',');
    word_number = std::stoi(word);
    getline(is, word, ',');
    getline(is, state, ',');
    if (ifs.eof() || ifs.bad()) throw std::runtime_error("Error reading statistics file");
    WordleGame game;
    game.word_number = word_number;
    game.word = word;
    while (!is.eof()) {
      getline(is, guess, ',');
      game.guesses.push_back(guess);
    }
    if (guess == word) game.state = won;
    else game.state = lost;

    if (game.state != unfinished) {
      if (game.state == won) {
        ++guess_frequencies.at(game.guesses.size() - 1);
      } else {
        ++guess_frequencies[7 - 1];
      }
    }
    games.emplace(word, game);
    getline(ifs, line);
  }
  if (ifs.eof()) return;  // we don't actually need the guess list
  getline(ifs, line); // "Unfinished Games"
  getline(ifs, line); // guess list header
  getline(ifs, line);
  while (!line.empty() && !ifs.eof()) {
    std::istringstream is;
    is.str(line);
    int word_number;
    char comma;
    string word;
    string guess;
    is >> word_number;
    is >> comma;
    getline(is, word, ',');
    if (ifs.eof() || ifs.bad()) throw std::runtime_error("Error reading statistics file");
    WordleGame game;
    game.word_number = word_number;
    game.word = word;
    while (!is.eof()) {
      getline(is, guess, ',');
      game.guesses.push_back(guess);
    }
    game.state = unfinished;
    games.emplace(word, game);
    unfinished_games.push_back(&(games[word]));
    getline(ifs, line);
  }
  ifs.close();
  return;
}

void WordleStatistics::writeToFile() {
  std::ofstream ofs(filename, std::ofstream::trunc);
  if (!ofs.is_open()) throw std::runtime_error("Couldn't create statistics file");
  
  // stats summary:
  ofs << "Bar chart only for 5-letter words\nNumber of guesses,Frequency\n";
  for (int i = 0; i < 7; ++i) {
    ofs << i + 1 << ',' << guess_frequencies[i] << '\n';
  }
  ofs << '\n';

  // guess record

  // categorize games
  std::vector<WordleGame> finished_games;
  std::vector<WordleGame> unfinished_games;
  for (auto pair : games) {
    WordleGame game = pair.second;
    if (game.state != unfinished) finished_games.push_back(game);
    else unfinished_games.push_back(game);
  }
  ofs << "Completed Games\n";
  ofs << "Wordle number (-1 for custom),Correct word,Won? (1 for yes; 0 for no),Guesses---->\n";
  if (!finished_games.empty()) {
    for (auto game : finished_games) {
      ofs << game.word_number << ',' << game.word << ',' << (game.state == won);
      for (auto guess : game.guesses) {
        ofs << ',' << guess;
      }
      ofs << '\n';
    }
  }
  ofs << '\n';

  if (!unfinished_games.empty()) {
    ofs << "Unfinished Games\n";
    ofs << "Wordle number,Correct word,Guesses---->\n";
    for (auto game : unfinished_games) {
      ofs << game.word_number << ',' << game.word;
      for (auto guess : game.guesses) {
        ofs << ',' << guess;
      }
      ofs << '\n';
    }
    ofs << '\n';
  }

  ofs.close();
}

void WordleStatistics::createFile() {
  std::ofstream ofs(filename);
  if (!ofs.is_open()) throw std::runtime_error("Couldn't create statistics file");

  // stats summary:
  ofs << "Bar chart only for 5-letter words\nNumber of guesses,Frequency\n";
  for (int i = 0; i < 7; ++i) {
    ofs << i + 1 << ',' << 0 << '\n';
  }
  ofs << '\n';

  ofs.close();
}

void WordleStatistics::addEntry(WordleWord word, std::vector<string> guesses) {
  if (guesses.size() == 0) return;
  GameState state;
  if (guesses.at(guesses.size() - 1) == word.word) state = won;
  else if (guesses.size() < 6) state = unfinished;
  else state = lost;
  WordleGame game = {word.number, word.word, guesses, state};
  if (state == won && word.word.size() == 5) ++guess_frequencies[guesses.size() - 1];
  else if (state == lost && word.word.size() == 5) ++guess_frequencies[7 - 1];
  if (!inFile(word)) {
    // completely new game never played before
    games.emplace(word.word, game);
  } else {
    // update status of previously played game
    if (state == won && word.word.size() == 5) {
      switch (games[word.word].state) {
        case won:
          --guess_frequencies[games[word.word].guesses.size() - 1];
          break;
        case lost:
          --guess_frequencies[7 - 1];
          break;
        default:
          break;
      }
    }
    games[word.word] = game;
  }
  writeToFile();
}

bool WordleStatistics::inFile(WordleWord word) {
  return games.count(word.word) != 0;
}

WordleGame WordleStatistics::loadEntry(WordleWord word) {
  if (!inFile(word)) {
    WordleGame game = {
      word.number, word.word, {}, unfinished
    };
    return game;
  }
  for (auto pair : games) {
    WordleGame game = pair.second;
    if (game.word == word.word) {
      return game;
    }
  }
  return WordleGame();
}

size_t WordleStatistics::numFinishedGames() {
  size_t num_finished = 0;
  for (auto pair : games) {
    WordleGame game = pair.second;
    if (game.state != unfinished) ++num_finished;
  }
  return num_finished;
}

double WordleStatistics::calcWinRate() {
  double num_won = 0.0;
  double num_finished = (double)games.size();
  for (auto pair : games) {
    WordleGame game = pair.second;
    if (game.state == won) ++num_won;
    else if (game.state == unfinished) --num_finished;
  }
  return num_won / num_finished;
}

double WordleStatistics::calcAverageNumGuesses() {
  double num_guesses = 0.0;
  double num_finished = 0.0;
  for (auto pair : games) {
    WordleGame game = pair.second;
    num_guesses += (double)game.guesses.size();
    if (game.state == lost) ++num_guesses;
    if (game.state != unfinished) ++num_finished;
  }
  return num_guesses / num_finished;
}
