
#pragma once
#include <boost/function/function_fwd.hpp>
#include <cstddef>
#include <vector>

#include "src/struct/BasicType.hpp"
namespace NAS {
namespace Struct {
template <typename Data>
class BasicTreeNote : public BasicType {
 public:
  Data data;
  BasicTreeNote<Data> *father;
  std::vector<BasicTreeNote<Data> *> children;
  ~BasicTreeNote() {
    for (auto &i : children) {
      delete i;
    }
    children.clear();
    std::vector<BasicTreeNote<Data> *> t;
    children.swap(t);
  }
  void forEach(boost::function<void(BasicTreeNote<Data> *)> func) {
    for (auto &i : children) {
      func(i);
    }
  }
  void DeepForEach(boost::function<void(BasicTreeNote<Data> *)> func) {
    for (auto &i : children) {
      func(i);
      i->DeepForEach(func);
    }
  }
  void add(BasicTreeNote<Data> *child) { children.push_back(child); }
};
template <typename Data>
class BasicTree : public BasicType {
 public:
  BasicTreeNote<Data> *root;
  BasicTree() {
    root = new BasicTreeNote<Data>;
    root->father = nullptr;
  }
  ~BasicTree() { delete root; }
  void DeepForEach(boost::function<void(BasicTreeNote<Data> *)> func) {
    root->DeepForEach(func);
  }
  void clear() { delete root; }
  void insert(BasicTreeNote<Data> *fa, BasicTree<Data> &tree) {
    tree.root->father = fa;
  }
};
}  // namespace Struct
}  // namespace NAS