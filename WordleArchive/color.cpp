#include "color.h"


namespace Color {
  bool Modifier::operator==(const Modifier& other) const {
    return fg_code == other.fg_code && bg_code == other.bg_code;
  }
  bool Modifier::operator!=(const Modifier& other) const {
    return !(*this == other);
  }
}
