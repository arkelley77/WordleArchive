#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// just put a * at the end of the word to be removed & it will be removed

std::vector<std::string> date_thing(std::string line) {
  size_t i = line.size() - 1;
  for (;; --i) {
    if (line[i] == ' ') break;
    if (i == 0) return {line, ""};
  }
  return {line.substr(0, i), line.substr(i + 1, line.size() - 1)};
}

int main() {
  std::ifstream ifs("wordles.txt");
  std::ostringstream oss;
  if (!ifs.is_open()) return 1;
  std::vector<std::string> removed_dates;
  std::string line;
  std::getline(ifs, line);
  while (!ifs.eof() && !line.empty()) {
    if (line.find('*') != std::string::npos) {
      // remove this word
      removed_dates.push_back(date_thing(line)[0]);
    } else {
      removed_dates.push_back(date_thing(line)[0]);
      oss << removed_dates[0] << ' ' << date_thing(line)[1] << '\n';
      removed_dates.erase(removed_dates.begin()); // pop front
    }
    std::getline(ifs, line);
  }
  oss << '\n';
  ifs.close();
  std::ofstream ofs("wordles.txt");
  if (!ofs.is_open()) return 1;
  ofs << oss.str();
  ofs.close();
}