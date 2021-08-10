// clang-format off
#include <samui.h>

#include <iostream>
#include <string>
// clang-format on

int main(int argc, char* argv[]) {
  SAMUI_ENGINE_ASSERT(argc == 2, "Usage: samui <project name>");
  std::cout << argv[1] << '\n';
  return 0;
}
