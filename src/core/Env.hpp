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
#include "src/struct/Tree.hpp"
namespace NAS {
namespace core {
namespace Parser {
enum class Type {
  TypeKeyWord,
  Start,
  KeyWord,
  Operator,
  Condition,
  Addon,
  Identifier,
  Const
};
/**
 * @brief Node for abstract syntax tree
 *
 */
class ASTNode : public Struct::BasicType {
 public:
  Type type;
  std::string key;
};
/**
 * @brief abstract syntax tree
 * @note for Parser
 */
namespace StandardKey {
const static std::string Start = "Start";
}
class AST : public Struct::BasicTree<ASTNode> {
 public:
  std::string toString() const override final {
    std::string str;
    root->DeepForEach([&str](Struct::BasicTreeNode<ASTNode> node) -> void {
      str += "[" + node.father->data.key + "]->[" + node.data.key + "]";
    });
    return str;
  }
};
/**
 * @brief BasicGrammar
 * @note for Parser
 *
 */

}  // namespace Parser
namespace Lexer {
/**
 * @brief Lexer Error
 *
 */

class LexerError : public Struct::BasicType {
 public:
  int code;
  /**
   * @brief get the error
   * @return std::string
   */
  virtual std::string what() const { return "Empty Error"; }
  std::string toString() const override { return what(); }
};

}  // namespace Lexer
namespace Env {
enum class State {
  Start,
  Letter,
  Number,
  Word,
  String,
  Operator,
  Note,
  LongNote
};
/*
 *For Word to control the compiler
 *<AddList> Add to the character stream
 *<back> Iter--;
 */
class BasicC {
 public:
  boost::function<void(Lexer::LexerError *)> Throw;
  boost::function<void(std::string)> addList;
  boost::function<void()> back;
};
enum class WordType {
  Number,
  Identifier,
  Note,
  Operator,
  Separator,
  KeyWord,
  String
};

class BasicsWord : public Struct::BasicType {
 public:
  WordType wordType;
  int id;
  std::string type;
  static bool isNumber(char Char) { return '0' <= Char && Char <= '9'; }
  static bool isLetter(char Char) {
    return ('a' <= Char && Char <= 'z') || ('A' <= Char && Char <= 'Z');
  }
  static bool isOperator(char Char) {
    switch (Char) {
      case '/':
      case '*':
      case '+':
      case '-':
      case '=':
      case '!':
      case '|':
      case '&':
      case '^':
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
      // controller.back();
      return false;
    }
    return false;
  }
  bool endLexer(std::string &str, State &state, BasicC &controller) override {
    if (state == State::Letter) state = State::Word;
    return false;
  }
};
/**
 * @brief BasicsLetter
 * @note work for Identifier
 *
 */
class BasicsLetter : public BasicsWord {
 public:
  BasicsLetter() {
    type = "identifier";
    id = 100;
    wordType = WordType::Identifier;
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
    wordType = WordType::KeyWord;
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
    wordType = WordType::Number;
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override final {
    if ((isNumber(Char) || (Char == '.' && state == State::Number)) &&
        (state == State::Start || state == State::Number)) {
      state = State::Number;
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
    wordType = WordType::String;
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
      controller.Throw(new StringEndErr);
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
    wordType = WordType::Operator;
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override final {
    if (isOperator(Char) &&
        (state == State::Start || state == State::Operator)) {
      str.push_back(Char);
      state = State::Operator;
      return true;
    } else if (state == State::Operator) {
      controller.addList(str);
      state = State::Start;
      str = "";
      controller.back();
      return false;
    }
    return false;
  }
};
class Note : public BasicsWord {
 public:
  Note() {
    type = "Note";
    id = 104;
    wordType = WordType::Operator;
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
  bool endLexer(std::string &str, State &state,
                BasicC &controller) override final {
    if (state == State::Note) {
      controller.addList(str);
      return true;
    }
    return false;
  }
};
class NoteErr : public Lexer::LexerError {
 public:
  std::string what() const override final { return "Note without a */"; }
};
class LongNote : public BasicsWord {
 public:
  LongNote() {
    type = "LongNote";
    id = 104;
    wordType = WordType::Note;
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override final {
    if (state == State::Operator && str.starts_with("/*")) {
      state = State::LongNote;
      return true;
    }
    if (state == State::LongNote) {
      str.push_back(Char);
      if (str.ends_with("*/")) {
        controller.addList(str);
        str = "";
        state = State::Start;
      }
      return true;
    }
    return false;
  }
  bool endLexer(std::string &str, State &state,
                BasicC &controller) override final {
    if (state == State::LongNote && !str.ends_with("*/")) {
      controller.Throw(new NoteErr);
      return true;
    }
    return false;
  }
};
class Separator : public BasicsWord {
 public:
  Separator() {
    id = 105;
    wordType = WordType::Separator;
    type = "Separator";
  }
  bool isSeparator(char Char) {
    switch (Char) {
      case ';':
      case ',':
      case '{':
      case '}':
      case '\n':
        return true;
      default:
        return false;
    }
  }
  bool checkLexer(char Char, std::string &str, State &state,
                  BasicC &controller) override {
    if (state == State::Start && isSeparator(Char)) {
      str.push_back(Char);
      controller.addList(str);
      str = "";
      return true;
    }
    return false;
  }
};
/*
 *for Compiler to config
 */

}  // namespace Env

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
}  // namespace Lexer
template <typename T>
concept AbleD = requires(T t) {
  t == t;
};
namespace Parser {
class ParserError : public Lexer::LexerError {
  std::string what() const override { return "[Empty][ParserError]"; }
};
class Controller {
 public:
  boost::function<void(ParserError *)> Throw;
};
enum class RunningState {
  Start,
  TypeKeyWord,
};
class BasicGrammar : public Struct::BasicType {
 public:
  template <AbleD T>
  static bool include(std::vector<T> list, T key) {
    for (auto i : list) {
      if (key == i) return true;
    }
    return false;
  }
  virtual bool check(Lexer::Lexicon &lexicon, AST &tree, RunningState state,
                     Struct::BasicTreeNode<ASTNode> *node,
                     Controller *controller) {
    return false;
  }
};
class TypeGrammarErr : public ParserError {
 public:
  std::string what() const override { return "[Grammar Error]def twice"; }
};
class TypeEmptyError : public ParserError {
 public:
  std::string what() const override { return "[Error]a empty Type define"; }
};
class TypeKeyWord : public BasicGrammar {
 public:
  std::vector<std::string> Keys;
  bool check(Lexer::Lexicon &lexicon, AST &tree, RunningState state,
             Struct::BasicTreeNode<ASTNode> *node,
             Controller *controller) override {
    if (state == RunningState::Start && include(Keys, lexicon.str)) {
      state = RunningState::TypeKeyWord;
      auto newNode = node->add(new Struct::BasicTreeNode<ASTNode>);
      newNode->data.type = Type::TypeKeyWord;
      newNode->data.key = lexicon.str;
      node = newNode;
      return true;
    }
    if (state == RunningState::TypeKeyWord) {
      if (lexicon.type->wordType == Env::WordType::Identifier) {
        if (node->children.size() == 0) {
          auto newNode = node->add(new Struct::BasicTreeNode<ASTNode>);
          newNode->data.type = Type::Identifier;
          newNode->data.key = lexicon.str;
        } else {
          controller->Throw(new TypeGrammarErr);
          state = RunningState::Start;
          node = node->father;
        }
      } else if (lexicon.type->wordType == Env::WordType::Separator) {
        if (node->children.size() == 0) {
          controller->Throw(new TypeEmptyError);
          state = RunningState::Start;
          node = node->father;
        } else {
          state = RunningState::Start;
          node = node->father;
        }
      } else if (lexicon.str == "=") {
        if (node->children.size() == 1) {
          auto newNode = node->add(new Struct::BasicTreeNode<ASTNode>);
          newNode->data.type = Type::Operator;
          newNode->data.key = "=";
        }
      } else if (lexicon.type->wordType == Env::WordType::Number ||
                 lexicon.type->wordType == Env::WordType::String) {
        if (node->children.size() == 2) {
          auto newNode =
              node->children[1]->add(new Struct::BasicTreeNode<ASTNode>);
          newNode->data.type = Type::Const;
          newNode->data.key = lexicon.str;
        }
      } else {
        state = RunningState::Start;
        node = node->father;
        return false;
      }
      return true;
    }
    return false;
  }
};
}  // namespace Parser
namespace Env {
class environment : public Struct::BasicType {
 public:
  std::string toString() const override {
    auto a = (std::string) "[Compiler Core][Environment]\n" +
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
  void clear() {
    for (auto i : wordList) {
      delete i;
    }
  }
};
}  // namespace Env
/**
 * @brief Standard Environment
 * @class StandardEnv
 * @note
 * TODD: more Keywords
 */
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
                new Env::BasicsLetter,
                new Env::Number,
                new Env::String,
                new Env::Operator,
                new Env::Note,
                new Env::LongNote};
  }
};
}  // namespace core
}  // namespace NAS