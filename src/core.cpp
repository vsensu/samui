#include "core.h"
#include <iostream>
#include "src_example/src_example.h"
#include "lib_example/lib_example.h"

namespace core {

void say(const char* str) {
  src_example::say(str);
}

void hello(){
  lib_example::hello();
}

}  // namespace core