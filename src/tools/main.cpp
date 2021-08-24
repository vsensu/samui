// clang-format off
#include <iostream>
#include <string>
// clang-format on

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: samui <project name>\n";
    return 0;
  }

  std::cout << argv[1] << '\n';
  return 0;
}
