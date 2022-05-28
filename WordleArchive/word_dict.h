//
//  word_dict.h
//  WordleArchive
//
//  Created by Alex Kelley on 5/14/22.
//

#ifndef word_dict_h
#define word_dict_h

#include "strtoupper.h"
#include "word_list.h"

#include <fstream>
#include <unordered_set>
#include <vector>
#include <string>

class WordDict {
  public:
    WordDict() : words_pointers(), valid_words(), filename() {}
    
    virtual ~WordDict() {}

    virtual void readFromFile(std::string filename);

    void readFromFile(std::ifstream& ifs);

    virtual void saveToFile();

    void saveToFile(std::ofstream& ofs);

    bool isWordInList(const std::string& word) const;

    inline WordleWord getRandomWord() {
      size_t index = rand() % words_pointers.size();
      return WordleWord(-2, -2, -2, -2, *words_pointers[index]);
    }

    void combine(WordList list);

    void combine(CustomWordList list);
  protected:
    std::vector<const std::string *> words_pointers;
    std::unordered_set<std::string> valid_words;
    std::string filename;
};

class CustomDict : public WordDict {
  public:
    void readFromFile(std::string filename);

    void createFile(std::string filename);

    void createFile(std::ofstream& ofs);

    void saveToFile();
};

#endif /* word_dict_h */
