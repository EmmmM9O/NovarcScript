#include "./Tree.hpp"

#include <memory>
template <typename T, typename Data>
NAS::Struct::_Node_<T, Data>::_Node_(std::unique_ptr<Data> data, T father) {
  this->data = data;
  this->father == father;
}
template <typename T, typename Data>
NAS::Struct::_Node_<T, Data>::_Node_(Data data, T father)
    : _Node_<T, Data>(std::make_unique(data), father) {}
template <typename T, typename Data>
NAS::Struct::_Node_<T, Data>::_Node_(T father)
    : _Node_<T, Data>(std::make_unique<Data>(nullptr), father) {}