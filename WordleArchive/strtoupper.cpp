#include "strtoupper.h"

std::string strToUpper(std::string str) {
  for (auto it = str.begin(); it != str.end(); ++it) {
    *it = toupper(*it);
  }
  return str;
}