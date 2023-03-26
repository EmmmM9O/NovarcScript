
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
