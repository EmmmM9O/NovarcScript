
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

class Lexicon : public Struct::BasicType {
 public:
  Env::BasicsWord *type;
  std::string str;
  std::string toString() const override {
    return "<" + type->toString() + ">:" + str;
  }
  Lexicon(std::string st, Env::BasicsWord *ty) {
    str = st;
    type = ty;
  }
};
class LexiconStream
    : public std::pair<std::vector<Lexicon>, std::vector<LexerError *> >,
      public Struct::BasicType {
 public:
  void clear() {
    first.clear();
    std::vector<Lexicon> t;
    first.swap(t);
    for (auto i : second) {
      delete i;
    }
    second.clear();
    std::vector<LexerError *> t2;
    second.swap(t2);
  }
  std::string toString() const override {
    std::string str;
    for (auto i : first) {
      str += "[" + i.toString() + "]\n";
    }
    str += Text::red.text("Error : " + std::to_string(second.size()));
    if (second.size() > 0) {
      for (auto i : second) {
        str += Text::red.text("\n[" + i->toString() + "]");
      }
    }
    return str;
  }
};

class _Lexer_ {
 public:
  Env::environment *env;
  LexiconStream run(std::string str) {
    LexiconStream stream;
    int Iter = 0;
    Env::BasicC controller;
    controller.back = [&Iter]() -> void { Iter--; };
    controller.Throw = [&stream](LexerError *err) -> void {
      stream.second.push_back(err);
    };
    Env::State state = Env::State::Start;
    std::string s;
    while (Iter < str.size()) {
      auto Char = str[Iter];
      Iter++;
      for (auto i : env->wordList) {
        controller.addList = [&i, &stream](std::string str) -> void {
          stream.first.push_back(Lexicon(str, i));
        };
        if (i->checkLexer(Char, s, state, controller)) {
          continue;
        }
      }
    }
    for (auto i : env->wordList) {
      controller.addList = [&i, &stream](std::string str) -> void {
        stream.first.push_back(Lexicon(str, i));
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
