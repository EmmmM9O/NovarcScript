#pragma once
#include <climits>
#include <string>

#include "BasicType.hpp"
namespace NAS {
namespace Struct {
class Color : public BasicType {
 public:
  int r, g, b;
  std::string toString() const override {
    return "2;" + std::to_string(r) + ";" + std::to_string(g) + ";" +
           std::to_string(b);
  }
  bool def;
  Color() {}
  Color(int r_, int g_, int b_) {
    r = r_;
    g = g_, b = b_;
    def = true;
  }
};
class Text : public BasicType {
 public:
  Color font, background;
  std::string toString() const override {
    return "\x1b[" + (font.def ? "38;" + font.toString() : "") +
           (background.def ? "48;" + background.toString() : "") + "m";
  }
  std::string text(std::string str) { return toString() + str + "\x1b[0m"; }
  Text(){};
  Text(Color fo) { font = fo; }
};
}  // namespace Struct
namespace Colors {
static Struct::Color red(255, 0, 0), blue(0, 0, 255), green(0, 255, 0);
}
namespace Text {
static Struct::Text red(Colors::red), blue(Colors::blue), green(Colors::green);
}
}  // namespace NAS
   // namespace NAS
