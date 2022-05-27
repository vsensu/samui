#pragma once

// clang-format off
#include <string>

#include "scene.h"
// clang-format on

namespace samui
{

namespace Serialization
{
void serialize_scene(Scene& scene, const std::string& filepath);
bool deserialize_scene(Scene& scene, const std::string& filepath);
}  // namespace Serialization
// class SAMUI_API SceneSerializer {
//  public:
//   SceneSerializer(Ref<Scene> scene);

//   private:
//   Ref<Scene> scene_;
// };
}  // namespace samui
