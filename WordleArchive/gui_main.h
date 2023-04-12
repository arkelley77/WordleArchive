#ifndef WORDLEARCHIVE_H
#define WORDLEARCHIVE_H

#include "gui.h"
#include "gui_config.h"
#include "word_list.h"
#include "word_dict.h"
#include "wordle_statistics.h"

#include <iostream>

#include <SFML/Graphics.hpp>

constexpr float view_width = 2000.f, view_height = 2000.f;
constexpr float outline_thickness = 10.f;
constexpr float gap_size = 40.f;

//enum MenuOption : char {
//  quit, word_by_number, word_by_date, random_wordle_word,
//  random_dict_word, custom_word, todays_word, next_word, home,
//  resume_game,
//};
enum GameMode : char {
  word_by_number, word_by_date, random_wordle_word,
  random_dict_word, custom_word, todays_word,
};
enum ActiveWindow : size_t {
  loading,
  main_menu,
  settings,
  game_select,
  word_select,
  in_game,
  pause_settings
};

struct ColorMode {
  sf::Color background;
  sf::Color outline;
  sf::Color button;
  sf::Color green;
  sf::Color yellow;
};

ColorMode lightMode() {
  return {
    sf::Color::White,
    sf::Color::Black,
    sf::Color(240, 240, 240),
    sf::Color::Green,
    sf::Color::Yellow
  };
}
ColorMode darkMode() {
  return {
    sf::Color::Black,
    sf::Color::White,
    sf::Color(15, 15, 15),
    sf::Color::Green,
    sf::Color::Yellow
  };
}

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
