#ifndef SAMUI_SERIALIZATION_H_
#define SAMUI_SERIALIZATION_H_

// clang-format off
#include <string>

#include <core/core.h>
#include "scene.h"
// clang-format on

namespace samui {

namespace Serialization {
void SAMUI_API SerializeScene(Scene& scene, const std::string& filepath);
bool SAMUI_API DeserializeScene(Scene &scene, const std::string& filepath);
}  // namespace Serialization
// class SAMUI_API SceneSerializer {
//  public:
//   SceneSerializer(Ref<Scene> scene);

//   private:
//   Ref<Scene> scene_;
// };
}  // namespace samui

#endif  // SAMUI_SERIALIZATION_H_