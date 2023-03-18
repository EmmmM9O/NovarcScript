#include <map>
#include <string>
namespace core {
namespace Lexer {
enum class State { Start, Letter, Operator,Note };
class _Lexer_ {
 public:
  virtual void intiKeyWord();
  std::map<std::string, int> KeyWordList;
  _Lexer_();
  std::string str;
  int Iter;
  char getChar();
  void run();
  bool isLetter(char Char);
  bool isNumber(char Char);
  int identifierValue;
  int notesValue;
};
}  // namespace Lexer
}  // namespace core