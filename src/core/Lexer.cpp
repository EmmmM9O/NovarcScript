#include "./Lexer.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
void core::Lexer::_Lexer_::intiKeyWord() {
  KeyWordList = {{"var", 1}, {"let", 2},  {"def", 3},
                 {"if", 4},  {"else", 5}, {"for", 6}};
}
core::Lexer::_Lexer_::_Lexer_() {
  intiKeyWord();
  Iter = 0;
  identifierValue = 100;
  notesValue = 101;
}
char core::Lexer::_Lexer_::getChar() {
  if (Iter >= str.size()) return (-1);
  Iter++;
  return str[Iter - 1];
}
bool core::Lexer::_Lexer_::isLetter(char Char) {
  return ('a' < Char && Char < 'z') || ('A' < Char && Char < 'Z');
}
bool core::Lexer::_Lexer_::isNumber(char Char) {
  return '0' < Char && Char < '9';
}
/*
 *<h1>run Lexer
 *state:
 *0:Start 1:read a Letter;
 *2:operator
 */
void core::Lexer::_Lexer_::run() {
  std::vector<std::pair<int, std::string>> list;
  std::string token;
  char Char;
  State state;
  do {
    Char = getChar();
  } while (Char == ' ' && (int)Char != -1);
  while ((int)Char != -1) {
    if (isLetter(Char) && state == State::Operator) {
      // start Read Letter
      token.push_back(Char);
      state = State::Letter;
      continue;
    }
    if (state == State::Letter) {
      // read Letter
      if (isLetter(Char) || isNumber(Char)) {
        token.push_back(Char);
        continue;
      } else {
        // search for KeyWord
        auto it = KeyWordList.find(token);
        if (it != KeyWordList.end()) {  // IsKey
          list.push_back(std::make_pair(it->second, it->first));
        } else {
          list.push_back(std::make_pair(identifierValue, token));
        }
        state = State::Start;
        Iter--;
        continue;
      }
    }
    if (state == State::Start && Char == '/') {
      token.push_back(Char);
      state = State::Operator;
      continue;
    }
    if (state == State::Operator && Char == '/') {
      state = State::Note;
      token.push_back(Char);
      continue;
    }
    if (state == State::Note) {
      if (Char == '\n') {
        list.push_back(std::make_pair(notesValue, token));
      } else {
        token.push_back(Char);
      }
      state = State::Start;
      continue;
    }
  }
  for (auto i : list) {
    std::cout << "{" << i.first << "," << i.second << "}" << std::endl;
  }
}