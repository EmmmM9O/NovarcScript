
#pragma once
#include <string>
#include <utility>
#include <vector>

#include "./Env.hpp"
#include "src/struct/BasicType.hpp"
#include "src/struct/Color.hpp"
namespace NAS {
namespace core {
namespace Lexer {

class Morpheme : public Struct::BasicType {
 public:
  Env::BasicsWord *type;
  std::string str;
  std::string toString() const override {
    return "<" + type->toString() + ">:" + str;
  }
  Morpheme(std::string st, Env::BasicsWord *ty) {
    str = st;
    type = ty;
  }
};
class MorphemeStream
    : public std::pair<std::vector<Morpheme>, std::vector<LexerError> >,
      public Struct::BasicType {
 public:
  std::string toString() const override {
    std::string str;
    for (auto i : first) {
      str += "[" + i.toString() + "]\n";
    }
    str += Text::red.text("Error : " + std::to_string(second.size()));
    if (second.size() > 0) {
      for (auto i : second) {
        str += Text::red.text("\n[" + i.toString() + "]");
      }
    }
    return str;
  }
};

class _Lexer_ {
 public:
  Env::environment *env;
  MorphemeStream run(std::string str) {
    MorphemeStream stream;
    int Iter = 0;
    Env::BasicC controller;
    controller.back = [&Iter]() -> void { Iter--; };
    Env::State state = Env::State::Start;
    std::string s;
    while (Iter < str.size()) {
      auto Char = str[Iter];
      Iter++;
      for (auto i : env->wordList) {
        controller.addList = [&i, &stream](std::string str) -> void {
          stream.first.push_back(Morpheme(str, i));
        };
        if (i->checkLexer(Char, s, state, controller)) {
          continue;
        }
      }
    }
    for (auto i : env->wordList) {
      controller.addList = [&i, &stream](std::string str) -> void {
        stream.first.push_back(Morpheme(str, i));
      };
      if (i->endLexer(s, state, controller)) {
        continue;
      }
    }
    return stream;
  }
};
}  // namespace Lexer
}  // namespace core
}  // namespace NAS
