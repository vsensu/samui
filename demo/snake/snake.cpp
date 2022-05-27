// clang-format off
#include "snake.h"

#include <engine/entrypoint.h>
// clang-format on

class Demo : public samui::GraphicsApplication
{
  public:
    Demo() : GraphicsApplication(samui::WindowProps("Snake", 1280, 720, false)) { push_layer(new Game2DLayer()); }
};

std::shared_ptr<samui::Application> samui::create_application()
{
    return std::make_shared<Demo>();
}
