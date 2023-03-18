#pragma once
#include <cstdio>
#include <memory>
#include <vector>
namespace NAS {
namespace Struct {
template <typename T, typename Data>
class _Node_ {
 public:
  std::unique_ptr<Data> data;
  T father;
  std::vector<T> children;
  _Node_(std::unique_ptr<Data> data, T father);
  _Node_(Data data, T father);
  _Node_(T father);
};
template <typename T, typename Data>
class _Tree_ {
 protected:
  std::vector<_Node_<T, Data>> nodes;
  _Node_<T, Data> *root;

 public:
};
}  // namespace Struct
}  // namespace NAS