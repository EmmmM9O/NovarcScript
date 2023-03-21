#pragma once
#include <array>
#include <boost/function.hpp>
#include <boost/function/function_fwd.hpp>
#include <streambuf>
#include <string>
#include <vector>

#include "src/struct/BasicType.hpp"
namespace NAS {
namespace core {
namespace Env {
enum class State { Start, Letter, Number, Word, String };
/*
 *For Word to control the compiler
 *<AddList> Add to the character stream
 *<back> Iter--;
 */
class BasicC {
 public:
  boost::function<void(std::string)> addList;
  boost::function<void()> back;
};
class BasicsWord : public Struct::BasicType {
 public:
  int id;
  std::string type;
  static bool isNumber(char Char) { return '0' <= Char && Char <= '9'; }
  static bool isLetter(char Char) {
    return ('a' <= Char && Char <= 'z') || ('A' <= Char && Char <= 'Z');
  }
  virtual std::string toString() const override {
    return "|" + std::to_string(id) + "|" + type;
  }
  /*
   *if true continue
   *
   */
  virtual bool checkLexer(char Char, std::string &str, State &state,
                          BasicC &controller) {
    return false;
  }
};
/*
 *Check whether it is a letter ,if true change
 *state into State::Word
 *work for Basics Letter
 *Priority higher than BasicsLetter
 */
class Letter : public BasicsWord {
 public:
  Letter() {
    type = "Letter";
    id = -1;
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override final {
    if ((isLetter(Char) || (isNumber(Char) && state == State::Letter)) &&
        (state == State::Letter || state == State::Start)) {
      state = State::Letter;
      str.push_back(Char);
      return true;
    } else if (state == State::Letter) {
      state = State::Word;
      controller.back();
      return false;
    }
    return false;
  }
};
/*
 *When a letter finish check it by State::Word
 *need Letter
 * also work for identifier
 *id 100 identifier
 */
class BasicsLetter : public BasicsWord {
 public:
  BasicsLetter() {
    type = "identifier";
    id = 100;
  }
  virtual bool checkWord(std::string &str) { return true; }
  virtual bool checkLexer(char Char, std::string &str, State &state,
                          BasicC &controller) override {
    if (state != State::Word) return false;
    if (checkWord(str)) {
      state = State::Start;
      controller.addList(str);
      str = "";
      return true;
    }
    return false;
  }
};
/*
 *Basics KeyWord
 *Work for keyWord check by 'key';
 */
class BasicsKeyWord : public BasicsLetter {
 public:
  std::string key;
  BasicsKeyWord(std::string k, int i) {
    key = k;
    id = i;
    type = "[KeyWord:" + k + "]";
  }
  virtual bool checkWord(std::string &str) override { return str == key; }
};
/*
 *Number,String are all types
 */
class Number : public BasicsWord {
  Number() {
    type = "Number";
    id = 101;
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override final {
    if ((isNumber(Char) || (Char == '.' && state == State::Number)) &&
        (state == State::Start || state == State::Number)) {
      str.push_back(Char);
      return true;
    } else if (state == State::Number) {
      controller.addList(str);
      state = State::Start;
      str = "";
      return false;
    }
  }
};
/*
 *for Compiler to config
 */
class environment : public Struct::BasicType {
 public:
  std::string toString() const override {
    auto a = (std::string) "[Compiler Core][Enviroment]\n" +
             (std::string) "-------list------\n";
    for (auto i : wordList) {
      a += "|" + std::to_string(i->id) + "|" + i->type + "|\n";
    }
    return a;
  }
  std::vector<BasicsWord *> wordList;
};
}  // namespace Env

namespace StandardEnv {
/*
 * standard Environment
 */
}
}  // namespace core
}  // namespace NAS