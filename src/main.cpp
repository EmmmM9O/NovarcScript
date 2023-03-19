
#include <fstream>
#include <iostream>
#include <string>
#include "core/Lexer.cpp"
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
    std::string str( (std::istreambuf_iterator<char>(in) ),
		    					 (std::istreambuf_iterator<char>() ) );
    std::cout<<"read:\n"<<str<<std::endl;
    in.close();
    core::Lexer::_Lexer_ Lexer;
    Lexer.str = str+"\n ";
    Lexer.run();
  }
  return 0;
}
