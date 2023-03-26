
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
    RunningState state = RunningState::Start;
    tree.root->data.type = Type::Start;
    tree.root->data.key = StandardKey::Start;
    auto nowNode = tree.root;
    return tree;
  }
};
}  // namespace Parser
}  // namespace core
}  // namespace NAS