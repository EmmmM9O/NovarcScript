#include <map>
#include <string>
#include <utility>
#include <vector>
namespace core {
namespace Lexer {
class LexerError {
 public:
  int code;
  virtual std::string what() { return "NoError"; };
};
namespace errors {
class NoteErr : public LexerError {
 public:
  std::string what() override;
  NoteErr();
};
class StringErr : public LexerError {
 public:
  StringErr();
  std::string what() override;
};
}  // namespace errors
enum class Type {
  identifier = 100,
  note = 101,
  number = 102,
  Error = 200,
  String = 103,
  KeyWord = -1,
  Operator = -2,
  Delimiter = -3
};
enum class State { Start, Letter, Operator, Note, LongNote, Number, String };
class Token {
 public:
  Type type;
  int code;
  std::string data;
  Token(Type t, int c, std::string d);
  Token(Type t, std::string d);
};
class _Lexer_ {
 public:
  virtual void intiKeyWord();
  virtual void initOperator();
  virtual void initDelimiter();
  std::map<std::string, int> KeyWordList;
  std::map<std::string, int> OperatorList;
  std::map<char, int> DelimiterList;
  _Lexer_();
  std::string str;
  int Iter;
  char getChar();
  std::pair<std::vector<Token>, std::vector<LexerError*>> run();
  bool isLetter(char Char);
  bool isNumber(char Char);
  bool isOperator(char Char);
};
}  // namespace Lexer
}  // namespace core
