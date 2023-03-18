#include "Tree.hpp"

#include <bits/types/FILE.h>

#include <boost/function/function_fwd.hpp>
#include <iostream>
#include <vector>
template <typename Data>
Struct::_Node_<Data>::_Node_() {}
template <typename Data>
Struct::_Node_<Data>::~_Node_<Data>() {
  for (auto i : children) {
    i = nullptr;
  } /*
   children.clear();
   auto a=std::vector < _Node_ < Data >*> ();
   children.swap(a);*/
}

template <typename Data>
Struct::_Node_<Data>::_Node_(Data _data) {
  data = _data;
}
template <typename Data>
void Struct::_Node_<Data>::add(_Node_<Data> *node) {
  children.push_back(node);
}
template <typename Data>
Struct::_Tree_<Data>::_Tree_() {
  nodes.push_back(_Node_<Data>());
  root = &nodes[0];
}
template <typename Data>
void Struct::_Tree_<Data>::print(std::ostream &stream) {
  print(stream, root);
}
template <typename Data>
void Struct::_Tree_<Data>::print(std::ostream &stream, _Node_<Data> *node) {
  stream << "|" << &node << "|";
  for (_Node_<Data> *i : node->children) {
    print(stream, i);
  }
}
template <typename Data>
Struct::_Tree_<Data>::~_Tree_<Data>() {
  nodes.clear();
  auto a = std::vector<_Node_<Data>>();
  nodes.swap(a);
}
template <typename Data>
void Struct::_Tree_<Data>::forEach(boost::function<void(_Node_<Data> *)> func) {
  forEach(func, root);
}
template <typename Data>
void Struct::_Tree_<Data>::forEach(boost::function<void(_Node_<Data> *)> func,
                                   _Node_<Data> *node) {
  func(node);
  for (_Node_<Data> *i : node->children) {
    func(i);
  }
}
