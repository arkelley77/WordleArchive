#include "word_dict.h"

void WordDict::readFromFile(std::string filename) {
  this->filename = filename;
  std::ifstream ifs(filename);
  if (!ifs.is_open()) throw std::runtime_error("couldn't open " + filename);
  readFromFile(ifs);
  ifs.close();
}

void WordDict::readFromFile(std::ifstream& ifs) {
  std::string word;
  std::getline(ifs, word);
  word = "";
  ifs >> word;
  while (ifs.good()) {
    valid_words.emplace(strToUpper(word));
    words_pointers.push_back(&*valid_words.find(strToUpper(word)));
    ifs >> word;
  }
}

bool WordDict::isWordInList(const std::string& word) const {
  return valid_words.find(strToUpper(word)) != valid_words.end();
}

void WordDict::combine(WordList list) {
  for (auto it = list.begin(); it != list.end(); ++it) {
    if (!isWordInList(it->second.word)) {
      valid_words.emplace(it->second.word);
      words_pointers.push_back(&*valid_words.find(strToUpper(it->second.word)));
    }
  }
  saveToFile();
}

void WordDict::combine(CustomWordList list) {
  for (auto it = list.begin(); it != list.end(); ++it) {
    if (!isWordInList(it->second.word)) {
      valid_words.emplace(it->second.word);
      words_pointers.push_back(&*valid_words.find(strToUpper(it->second.word)));
    }
  }
  saveToFile();
}

void WordDict::saveToFile() {
  std::ofstream ofs(filename);
  saveToFile(ofs);
  ofs.close();
}

void WordDict::saveToFile(std::ofstream& ofs) {
  for (auto it = words_pointers.begin(); it != words_pointers.end(); ++it) {
    ofs << **it << '\n';
  }
}

void CustomDict::readFromFile(std::string filename) {
  std::ifstream ifs(filename);
  std::string ignore;
  if (!ifs.is_open()) {
    createFile(filename);
    ifs.open(filename);
  }
  std::getline(ifs, ignore);
  std::getline(ifs, ignore);
  std::getline(ifs, ignore);
  WordDict::readFromFile(ifs);
  ifs.close();
}

void CustomDict::createFile(std::string filename) {
  std::ofstream ofs(filename);
  createFile(ofs);
  ofs.close();
}

void CustomDict::createFile(std::ofstream& ofs) {
  ofs << "This is the custom dictionary. Place any words that should be valid\n";
  ofs << "guesses for your custom word list into this list. Don't forget a\n";
  ofs << "blank line at the end of the file." << std::endl;
}

void CustomDict::saveToFile() {
  std::ofstream ofs(filename);
  createFile(ofs);
  WordDict::saveToFile(ofs);
  ofs.close();
}