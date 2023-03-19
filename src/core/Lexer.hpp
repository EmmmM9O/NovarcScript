#include <map>
#include <string>
namespace core {
namespace Lexer {
enum class Type {
  identifier = 100,
  note = 101,
  number = 102,
  Error = 200,
  String = 103
};
enum class State { Start, Letter, Operator, Note, LongNote, Number, String };
class _Lexer_ {
 public:
  virtual void intiKeyWord();
  virtual void initOperator();
  std::map<std::string, int> KeyWordList;
  std::map<std::string, int> OperatorList;
  _Lexer_();
  std::string str;
  int Iter;
  char getChar();
  void run();
  bool isLetter(char Char);
  bool isNumber(char Char);
  bool isOperator(char Char);
};
}  // namespace Lexer
}  // namespace core