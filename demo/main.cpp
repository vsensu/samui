#include <iostream>
#include "core.h"

int main() {
  core::say("hello demo\n");
  std::cout << core::add(1, 2) << '\n';
  core::hello();
}