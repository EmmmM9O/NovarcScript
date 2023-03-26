
#pragma once
#include <boost/function.hpp>
#include <vector>

#include "src/struct/BasicType.hpp"
namespace NAS {
namespace Struct {
template <typename Data>
class BasicTreeNode : public BasicType {
 public:
  Data data;
  BasicTreeNode<Data> *father;
  std::vector<BasicTreeNode<Data> *> children;
  ~BasicTreeNode() {
    for (auto &i : children) {
      delete i;
    }
    children.clear();
    std::vector<BasicTreeNode<Data> *> t;
    children.swap(t);
  }
  void forEach(boost::function<void(BasicTreeNode<Data> *)> func) {
    for (auto &i : children) {
      func(i);
    }
  }
  void DeepForEach(boost::function<void(BasicTreeNode<Data> *)> func) {
    for (auto &i : children) {
      func(i);
      i->DeepForEach(func);
    }
  }
  BasicTreeNode<Data> *add(BasicTreeNode<Data> *child) {
    children.push_back(child);
    return children[children.size() - 1];
  }
};
template <typename Data>
class BasicTree : public BasicType {
 public:
  virtual std::string toString() const override { return "[Tree][Object]"; }
  BasicTreeNode<Data> *root;
  BasicTree() {
    root = new BasicTreeNode<Data>;
    root->father = nullptr;
  }
  ~BasicTree() { delete root; }
  void DeepForEach(boost::function<void(BasicTreeNode<Data> *)> func) {
    root->DeepForEach(func);
  }
  void clear() { delete root; }
  void insert(BasicTreeNode<Data> *fa, BasicTree<Data> &tree) {
    tree.root->father = fa;
  }
};
}  // namespace Struct
}  // namespace NAS