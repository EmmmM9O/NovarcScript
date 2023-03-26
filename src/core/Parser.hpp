
#pragma once
#include <src/core/Env.hpp>
#include <src/core/Lexer.hpp>
#include <src/struct/BasicType.hpp>
#include <src/struct/Tree.hpp>
namespace NAS {
namespace core {
namespace Parser {
/**
 * @brief Syntactic analysis
 *
 */
class _parser_ : public Struct::BasicType {
 public:
  Env::environment *env;
  /**
   * @brief
   *
   * @param stream  LexiconStream
   * @return AST Abstract Syntax Tree
   */
  AST run(Lexer::LexiconStream stream) {
    AST tree;
    tree.root->data.type = Type::Start;
    tree.root->data.key = StandardKey::Start;
  }
};
}  // namespace Parser
}  // namespace core
}  // namespace NAS