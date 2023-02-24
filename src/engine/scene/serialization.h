#pragma once

// clang-format off
#include <string>

#include "scene_module.h"
#include "scene.h"
// clang-format on

namespace samui
{

namespace Serialization
{
void SAMUI_SCENE_API serialize_scene(Scene& scene, const std::string& filepath);
bool SAMUI_SCENE_API deserialize_scene(Scene& scene, const std::string& filepath);
}  // namespace Serialization
// class SAMUI_API SceneSerializer {
//  public:
//   SceneSerializer(Ref<Scene> scene);

//   private:
//   Ref<Scene> scene_;
// };
}  // namespace samui
