//
//  color.h
//  WordleArchive
//
//  Created by Alex Kelley on 5/14/22.
//

#ifndef color_h
#define color_h

#include <ostream>

namespace Color {
  enum Code {
    NONE = 0,
    FG_BLACK    = 30,
    FG_RED      = 31,
    FG_WHITE    = 37,
    FG_DEFAULT  = 39,
    BG_BLACK    = 40,
    BG_GREEN    = 42,
    BG_YELLOW   = 43,
    BG_WHITE    = 47,
    BG_DEFAULT  = 49
  };
  class Modifier {
    Code fg_code;
    Code bg_code;
  public:
    Modifier() : fg_code(FG_DEFAULT), bg_code(BG_DEFAULT) {}
    Modifier(Code fg_code, Code bg_code) : fg_code(fg_code), bg_code(bg_code) {}
    bool operator==(const Modifier& other) const;
    bool operator!=(const Modifier& other) const;
      friend std::ostream&
      operator<<(std::ostream& os, const Modifier& mod) {
        if (mod.fg_code != NONE) os << "\033[" << mod.fg_code << "m";
        if (mod.bg_code != NONE) os << "\033[" << mod.bg_code << "m";
        return os;
      }
  };
  const Modifier defaults(FG_DEFAULT, BG_DEFAULT);
  const Modifier error(FG_RED, BG_DEFAULT);
  const Modifier wrong(FG_WHITE, BG_BLACK);
  const Modifier wrong_place(FG_BLACK, BG_YELLOW);
  const Modifier correct(FG_WHITE, BG_GREEN);
  const Modifier grey(FG_BLACK, BG_WHITE);
}

#endif /* color_h */
