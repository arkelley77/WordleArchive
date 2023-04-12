#ifndef GUI_CONFIG_H
#define GUI_CONFIG_H

#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>

struct GUIConfig {
  unsigned int width;
  unsigned int height;
  unsigned int bit_depth;
  bool dark_mode;
  GUIConfig() : width(800), height(800), bit_depth(32), dark_mode(false) {}
  
  void readFromFile(const std::string filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) return;
    ifs >> width;
    ifs >> height;
    ifs >> bit_depth;
    ifs >> dark_mode;
    ifs.close();
  }
  void writeToFile(std::string filename) {
    std::ofstream ofs(filename);
    ofs << width << '\n';
    ofs << height << '\n';
    ofs << bit_depth << '\n';
    ofs << dark_mode << '\n';
    ofs.close();
  }

  sf::VideoMode getVideoMode() const { return sf::VideoMode(width, height, bit_depth); }
};

#endif