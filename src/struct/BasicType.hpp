#pragma once
#include <iostream>
#include <string>
#include <typeinfo>
namespace NAS {
namespace Struct {
class BasicType {
 public:
  virtual long int Class() { return typeid(BasicType).hash_code(); }
  virtual std::string toString() const { return "[object][obejct]"; }
  friend std::ostream &operator<<(std::ostream &stream, const BasicType &t);
};
}  // namespace Struct
}  // namespace NAS
inline std::ostream &operator<<(std::ostream &stream,
                                const NAS::Struct::BasicType &t) {
  std::string a = t.toString();
  return stream;
}