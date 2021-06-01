#ifndef CORE_H_
#define CORE_H_

#include "math/math.h"
#include "src_example/src_example.h"

namespace core {

void say(const char* str);

template <typename T1, typename T2>
auto add(const T1& lhs, const T2& rhs) {
  return math::add(lhs, rhs);
}

void hello();

}  // namespace core

#endif