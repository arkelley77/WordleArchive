//
//  word_list.h
//  WordleArchive
//
//  Created by Alex Kelley on 5/14/22.
//

#ifndef word_list_h
#define word_list_h

#include "strtoupper.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <array>
#include <string>
#include <sstream>

const std::array<std::string, 12> month_names = {"January", "February", "March",
  "April", "May", "June", "July", "August", "September", "October", "November",
  "December"};
const std::array<std::string, 12> month_abrevs = {"Jan", "Feb", "Mar", "Apr",
  "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int Date(int month, int day, int year);

int daysInMonth(int month);

int find(const std::array<std::string, 12>& arr, const std::string& value);

int monthToInt(std::string& month);

class WordleWord {
  public:
    WordleWord() : month(-4), day(-4), year(-4), number(-4), word() {}
    WordleWord(std::string word) 
    : month(), day(-1), year(-1), number(-1), word(word) {}
    WordleWord(int month, int day, int year, int number, std::string word)
    : month(month), day(day), year(year), number(number), word(word) {}
    WordleWord(std::string month, int day, int year, int number, std::string word)
    : month(monthToInt(month)), day(day), year(year), number(number), word(word) {}

    std::string getHeader() const;

    std::string getMonthName() const { return month_names.at(month); }
    std::string getMonthAbbrev() const { return month_abrevs.at(month); }
    int month;
    int day;
    int year;
    int number;
    std::string word;
};

class WordList {
  public:
    WordList() : word_list_by_number(), word_list_by_date() {}

    void readFromFile(std::string filename);

    inline WordleWord getWordByNumber(const int number) const {
      return word_list_by_number.at(number);
    }

    inline WordleWord getWordByDate(const int date) const {
      return word_list_by_date.at(date);
    }

    inline WordleWord getRandomWord() const {
      return word_list_by_number.at(rand() % word_list_by_number.size());
    }

    inline size_t size() const { return word_list_by_number.size(); }

    auto begin() {  return word_list_by_number.begin(); }

    auto end() { return word_list_by_number.end(); }
  private:
    // lookup by word number
    std::unordered_map<int, WordleWord> word_list_by_number;
    std::unordered_map<int, WordleWord> word_list_by_date;
};

class CustomWordList {
  public:
    CustomWordList() : word_pointers(), word_list() {}

    void readFromFile(std::string filename);

    void createFile(std::string filename);

    inline WordleWord getRandomWord() {
      return *word_pointers.at(rand() % word_pointers.size());
    }

    bool empty() const { return word_list.empty(); }
    std::vector<WordleWord>::size_type size() const { return word_list.size(); }

    auto begin() {  return word_list.begin(); }

    auto end() { return word_list.end(); }
  private:
    std::vector<WordleWord*> word_pointers;
    std::unordered_map<std::string, WordleWord> word_list;
};

class NewWordList {
public:
  NewWordList() {}

  void readFromFile(std::string filename);

  WordleWord getWordByNumber(int number) {
    return words.at(number - 1);
  }

  WordleWord getRandomWord() {
    return words[rand() % size()];
  }

  bool empty() const { return words.empty(); }
  size_t size() const { return words.size(); }

private:
  std::vector<WordleWord> words;
};

#endif /* word_list_h */
