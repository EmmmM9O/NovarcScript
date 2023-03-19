#pragma once
#include "./Lexer.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
core::Lexer::Token::Token(core::Lexer::Type t, int i, std::string d) {
  type = t;
  code = i;
  data = d;
}
core::Lexer::Token::Token(core::Lexer::Type t, std::string d)
    : Token(t, (int)t, d) {}
std::string core::Lexer::errors::StringErr::what() {
  return "String end without \"";
}
core::Lexer::errors::StringErr::StringErr() { code = 11; }
std::string core::Lexer::errors::NoteErr::what() {
  return "Note Don't end in the end";
}
void core::Lexer::_Lexer_::initDelimiter() {
  DelimiterList = {{'{', 201}, {'}', 202}, {';', 203}, {',', 204}};
}
core::Lexer::errors::NoteErr::NoteErr() { code = 10; }
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
  initDelimiter();
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
std::pair<std::vector<core::Lexer::Token>,
          std::vector<core::Lexer::LexerError*>>
core::Lexer::_Lexer_::run() {
  std::vector<Token> list;
  std::vector<LexerError*> errorList;
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
        list.push_back(Token(Type::number, token));
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
      auto it = DelimiterList.find(Char);
      if (it != DelimiterList.end()) {
        std::string a;
        a += it->first;
        list.push_back(Token(Type::Delimiter, it->second, a));
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
          list.push_back(Token(Type::KeyWord, it->second, it->first));
        } else {
          list.push_back(Token(Type::identifier, token));
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
        Iter--;
        auto it = OperatorList.find(token);
        if (it != OperatorList.end()) {
          list.push_back(Token(Type::Operator, it->second, token));
        } else {
          list.push_back(Token(Type::Error, token));
        }
        token = "";
        state = State::Start;
      }
      continue;
    }

    if (state == State::LongNote) {
      token.push_back(Char);
      if (token.ends_with("*/")) {
        list.push_back(Token(Type::note, token));
        token = "";
        state = State::Start;
      } else if (Iter == str.size() - 1) {
        errorList.push_back(new errors::NoteErr{});
      }
      continue;
    }
    if (state == State::Note) {
      if (Char == '\n' || Iter == str.size() - 1) {
        list.push_back(Token(Type::note, token));
        token = "";
        state = State::Start;
      } else {
        token.push_back(Char);
      }
      continue;
    }
    if (state == State::String) {
      token.push_back(Char);
      if (Char == '\"') {
        list.push_back(Token(Type::String, token));
        token = "";
        state = State::Start;
      } else if (Iter == str.size() - 1) {
        errorList.push_back(new errors::StringErr{});
      }
    }
  }
  for (auto i : list) {
    std::cout << "{" << i.code << "," << i.data << "}" << std::endl;
  }
  for (auto i : errorList) {
    std::cout << "|code:" << i->code << "|" << i->what() << std::endl;
    delete i;
  }
}
