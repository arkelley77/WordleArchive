#include "word_list.h"

int Date(int month, int day, int year) {
  return month * 1000000
        +  day *   10000
        + year *       1;
}

int daysInMonth(int month) {
  switch (month) {
    case 0:
    case 2:
    case 4:
    case 6:
    case 7:
    case 9:
    case 11:
      return 31;
    case 3:
    case 5:
    case 8:
    case 10:
      return 30;
    case 1:
      return 29;
    default:
      return -1;
  }
}

int find(const std::array<std::string, 12>& arr, const std::string& value) {
  for (int i = 0; i < 12; ++i) {
    if (value == arr[i]) return i;
  }
  return -1;
}
int monthToInt(std::string& month) {
  if (month.size() >= 3) {
    month.at(0) = toupper(month.at(0));
    for (std::string::size_type i = 1; i < month.size(); ++i) {
      month.at(i) = tolower(month.at(i));
    }
    return find(month_abrevs, month.substr(0, 3));
  } else {
    try {
      return stoi(month) - 1;
    } catch (const std::exception& e) {
      return -1;
    }
  }
}

std::string WordleWord::getHeader() const {
  std::ostringstream os;
  if (day == -2) {
    os << "Random Wordle" << std::endl;
  }else if (day == -1) {
    os << "Custom Wordle: ";
    os << word.size() << " letters" << std::endl;
  } else {
    os << "Wordle #" << number << ", ";
    os << getMonthName() << ' ' << day << ' ' <<  year << std::endl;
  }
  return os.str();
}

void WordList::readFromFile(std::string filename) {
  std::ifstream ifs(filename);
  if (!ifs.is_open()) throw std::runtime_error("couldn't open " + filename);
  // line format is:
  // MMM DD YYYY Day .. ~~~~~
  // where .. is the word number & ~~~~~ is the word
  std::string mmm;
  int dd, yyyy;
  int word_number;
  std::string word;

  ifs >> mmm >> dd >> yyyy >> word_number >> word;
  while (ifs.good()) {
    WordleWord wordle_word = {mmm, dd, yyyy, word_number, word};
    word_list_by_number.emplace(word_number, wordle_word);
    word_list_by_date.emplace(Date(monthToInt(mmm), dd, yyyy), wordle_word);

    ifs >> mmm >> dd >> yyyy >> word_number >> word;
  }
  ifs.close();
}

void CustomWordList::readFromFile(std::string filename) {
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    createFile(filename);
    ifs.open(filename);
  }
  std::string word;
  std::getline(ifs, word);  // throw away header lines
  word = "";
  ifs >> word;
  while (ifs.good()) {
    word_list.emplace(strToUpper(word), WordleWord(strToUpper(word)));
    word_pointers.push_back(&word_list[strToUpper(word)]);
    ifs >> word;
  }
  ifs.close();
}

void CustomWordList::createFile(std::string filename) {
  std::ofstream ofs(filename);
  ofs << "This is the custom word list. Put each new word on a new line, ";
  ofs << "then end the file with a blank line." << std::endl;
  ofs.close();
}
