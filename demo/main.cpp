#include <iostream>
#include <samui.h>

samui::Application* samui::create_application()
{
  std::cout << "Hello samui\n";
  return new samui::Application();
}