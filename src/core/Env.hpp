#pragma once
#include <array>
#include <boost/function.hpp>
#include <boost/function/function_fwd.hpp>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

#include "src/struct/BasicType.hpp"
#include "src/struct/Color.hpp"
namespace NAS {
namespace core {
namespace Lexer {
class LexerError : public Struct::BasicType {
 public:
  int code;
  virtual std::string what() const { return "Empty Error"; }
  std::string toString() const override { return what(); }
};
}  // namespace Lexer
namespace Env {
enum class State { Start, Letter, Number, Word, String, Operator, Note };
/*
 *For Word to control the compiler
 *<AddList> Add to the character stream
 *<back> Iter--;
 */
class BasicC {
 public:
  boost::function<void(Lexer::LexerError)> Throw;
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
  static bool isOpearter(char Char) {
    switch (Char) {
      case '/':
      case '*':
      case '+':
      case '-':
        return true;
      default:
        return false;
    }
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
  virtual bool endLexer(std::string &str, State &state, BasicC &controller) {
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
  bool endLexer(std::string &str, State &state, BasicC &controller) override {
    if (state == State::Letter) state = State::Word;
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
 public:
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
    return false;
  }
};
class StringEndErr : public Lexer::LexerError {
 public:
  std::string what() const override final { return "string end without a \""; }
};
class String : public BasicsWord {
 public:
  String() {
    type = "String";
    id = 102;
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override final {
    if (state == State::Start && Char == '\"') {
      state = State::String;
      str += Char;
      return true;
    }
    if (state == State::String) {
      str += Char;
      if (Char == '\"') {
        controller.addList(str);
        str = "";
        state = State::Start;
      }
      return true;
    }
    return false;
  }
  bool endLexer(std::string &str, State &state, BasicC &controller) override {
    if (state == State::String && !str.ends_with("\"")) {
      controller.Throw(StringEndErr());
      return true;
    }
    return false;
  }
};
class Operator : public BasicsWord {
 public:
  Operator() {
    type = "Operator";
    id = 103;
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override final {
    if (isOpearter(Char) &&
        (state == State::Start || state == State::Operator)) {
      str.push_back(Char);
      state = State::Operator;
    } else if (state == State::Operator) {
      controller.addList(str);
      state = State::Start;
      str = "";
    }
    return false;
  }
};
class Note : public BasicsWord {
 public:
  Note() {
    type = "Note";
    id = 104;
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override final {
    if (state == State::Operator && str.starts_with("//")) {
      state = State::Note;
      return true;
    } else if (state == State::Note) {
      if (Char == '\n') {
        controller.addList(str);
        str = "";
        state = State::Start;
      } else {
        str.push_back(Char);
      }
      return true;
    }
    return false;
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
  ~environment() {
    for (auto i : wordList) {
      delete i;
    }
  }
};
}  // namespace Env

class StandardEnv : public Env::environment {
 public:
  /*
   * standard Environment
   */
  StandardEnv() {
    wordList = {new Env::Letter,
                new Env::BasicsKeyWord("def", 1),
                new Env::BasicsKeyWord("var", 2),
                new Env::BasicsKeyWord("if", 3),
                new Env::BasicsKeyWord("else", 4),
                new Env::BasicsKeyWord("do", 5),
                new Env::BasicsKeyWord("while", 6),
                new Env::Number,
                new Env::String,
                new Env::Operator,
                new Env::Note};
  }
};
}  // namespace core
}  // namespace NAS