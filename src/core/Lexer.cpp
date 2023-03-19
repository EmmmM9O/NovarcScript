#include "./Lexer.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
void core::Lexer::_Lexer_::intiKeyWord() {
  KeyWordList = {{"var", 1}, {"let", 2},  {"def", 3},
                 {"if", 4},  {"else", 5}, {"for", 6}};
}
void core::Lexer::_Lexer_::initOperator() {
  OperatorList = {{"(", 51},  {")", 52}, {"[", 53}, {"]", 54},  {"!", 55},
                  {"^", 56},  {"|", 57}, {"&", 58}, {"&&", 59}, {"||", 60},
                  {"==", 61}, {">", 62}, {"<", 63}, {"<=", 64}, {">=", 65},
                  {"%", 66},  {"/", 67}, {"*", 68}, {"+", 69},  {"-", 70},
                  {"*", 71},  {"=", 72}};
}
core::Lexer::_Lexer_::_Lexer_() {
  intiKeyWord();
  initOperator();
  Iter = 0;
}
char core::Lexer::_Lexer_::getChar() {
  if (Iter >= str.size()) throw "Out";
  Iter++;
  return str[Iter - 1];
}
bool core::Lexer::_Lexer_::isLetter(char Char) {
  return ('a' <= Char && Char <= 'z') || ('A' <= Char && Char <= 'Z');
}
bool core::Lexer::_Lexer_::isNumber(char Char) {
  return '0' <= Char && Char <= '9';
}
bool core::Lexer::_Lexer_::isOperator(char Char) {
  switch (Char) {
    case '+':
    case '-':
    case '/':
    case '*':
    case '>':
    case '<':
    case '=':
    case '(':
    case ')':
    case '[':
    case ']':
    case '|':
    case '&':
      return true;
    default:
      return false;
  }
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
  State state = State::Start;
  do {
    try {
      Char = getChar();
    } catch (...) {
      break;
    }
  } while (Char == ' ' && (int)Char != -1);
  Iter--;
  while (true) {
    try {
      Char = getChar();
    } catch (...) {
      break;
    }
    if (state == State::Number) {
      if (isNumber(Char)) {
        token.push_back(Char);
      } else {
        list.push_back(std::make_pair((int)Type::number, token));
        state = State::Start;
        token = "";
      }
      continue;
    }
    if (state == State::Start) {
      if (isNumber(Char)) {
        token.push_back(Char);
        state = State::Number;
        continue;
      }
      if (isLetter(Char)) {
        token.push_back(Char);
        state = State::Letter;
        continue;
      }
      if (isOperator(Char)) {
        token.push_back(Char);
        state = State::Operator;
        continue;
      }
      if (Char == '\"') {
        token.push_back(Char);
        state = State::String;
        continue;
      }
    }
    if (state == State::Letter) {
      // read Letter
      if ((isLetter(Char) || isNumber(Char)) && Iter < str.size()) {
        // std::cout<<"push :"<<Char<<std::endl;
        token.push_back(Char);
        continue;
      } else {
        if (isLetter(Char) || isNumber(Char)) token.push_back(Char);
        // search for KeyWord
        auto it = KeyWordList.find(token);
        if (it != KeyWordList.end()) {  // IsKey
          list.push_back(std::make_pair(it->second, it->first));
        } else {
          list.push_back(std::make_pair((int)Type::identifier, token));
        }
        state = State::Start;
        Iter--;
        token = "";
        continue;
      }
    }
    if (state == State::Operator) {
      if (Char == '/') {
        state = State::Note;
        token.push_back(Char);
        continue;
      }
      if (Char == '*') {
        state = State::LongNote;
        token.push_back(Char);
        continue;
      }
      if (isOperator(Char)) {
        token.push_back(Char);
      }
      if (!isOperator(Char) || Iter == str.size() - 1) {
        auto it = OperatorList.find(token);
        if (it != OperatorList.end()) {
          list.push_back(std::make_pair(it->second, token));
        } else {
          list.push_back(std::make_pair((int)Type::Error, token));
        }
        token = "";
        state = State::Start;
      }
      continue;
    }

    if (state == State::LongNote) {
      token.push_back(Char);
      if (token.ends_with("*/") || Iter == str.size() - 1) {
        list.push_back(std::make_pair((int)Type::note, token));
        token = "";
        state = State::Start;
      }
      continue;
    }
    if (state == State::Note) {
      if (Char == '\n' || Iter == str.size() - 1) {
        list.push_back(std::make_pair((int)Type::note, token));
        token = "";
        state = State::Start;
      } else {
        token.push_back(Char);
      }
      continue;
    }
    if (state == State::String) {
      token.push_back(Char);
      if (Char == '\"' || Iter == str.size() - 1) {
        list.push_back(std::make_pair((int)Type::String, token));
        token = "";
        state = State::Start;
      }
    }
  }
  for (auto i : list) {
    std::cout << "{" << i.first << "," << i.second << "}" << std::endl;
  }
}
