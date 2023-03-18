#pragma once
#include <boost/function.hpp>
#include <boost/function/function_fwd.hpp>
#include <ostream>
#include <vector>
namespace Struct {
template <typename Data>
class _Node_ {
 public:
  Data data;
  std::vector<_Node_<Data>*> children;
  _Node_(Data data);
  _Node_();
  void add(_Node_<Data>* node);
  ~_Node_();
};
template <typename Data>
class _Tree_ {
 public:
  std::vector<_Node_<Data>> nodes;
  _Node_<Data>* root;
  _Tree_();
  ~_Tree_();
  void print(std::ostream& stream);
  void print(std::ostream& stream, _Node_<Data>* node);
  void forEach(boost::function<void(_Node_<Data>*)> func);
  void forEach(boost::function<void(_Node_<Data>*)> func, _Node_<Data>* node);
};

}  // namespace Struct