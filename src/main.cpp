
#include <fstream>
#include <iostream>
#include <string>

#include "src/core/Env.hpp"
#include "src/core/Lexer.hpp"
int main(int argc, char** args) {
  if (argc < 2) {
    std::cout << "No File";
  } else {
    auto path = args[1];
    std::ifstream in(path);
    if (!in) {
      std::cout << "No such file";
      return 0;
    }
    std::string str((std::istreambuf_iterator<char>(in)),
                    (std::istreambuf_iterator<char>()));

    std::cout << "read:\n-------\n" << str << std::endl
              << "---------" << std::endl;
    NAS::core::Lexer::_Lexer_ lexer;
    lexer.env = new NAS::core::StandardEnv;
    auto s = lexer.run(str);
    std::cout<<"Key Words:"<<lexer.env->toString()<<"---------\n" << s.toString()<<std::endl;
    s.clear();
  }
  return 0;
}
