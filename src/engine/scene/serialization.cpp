// clang-format off
#include "serialization.h"

#include <fstream>
#include <sstream>

#include <yaml-cpp/yaml.h>
#include "components.h"
// clang-format on

namespace YAML {

template <>
struct convert<glm::vec3> {
  static Node encode(const glm::vec3& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::vec3& rhs) {
    if (!node.IsSequence() || node.size() != 3) return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
  }
};

template <>
struct convert<glm::vec4> {
  static Node encode(const glm::vec4& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.push_back(rhs.w);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::vec4& rhs) {
    if (!node.IsSequence() || node.size() != 4) return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    rhs.w = node[3].as<float>();
    return true;
  }
};

}  // namespace YAML

namespace samui {

namespace Serialization {
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
  return out;
}

void SerializeEntity(YAML::Emitter& out, Scene& scene, Entity entity) {
  out << YAML::BeginMap;  // Entity Map
  out << YAML::Key << "Entity";
  out << YAML::Value << "123456789";

  if (scene.HasComponent<NameComponent>(entity)) {
    out << YAML::Key << "NameComponent";
    out << YAML::BeginMap;  // NameComponent Map

    auto& name = scene.GetComponent<NameComponent>(entity).name;
    out << YAML::Key << "Name" << YAML::Value << name;

    out << YAML::EndMap;  // NameComponent Map
  }

  if (scene.HasComponent<TransformComponent>(entity)) {
    out << YAML::Key << "TransformComponent";
    out << YAML::BeginMap;  // TransformComponent Map

    auto& tc = scene.GetComponent<TransformComponent>(entity);
    out << YAML::Key << "Translation" << YAML::Value << tc.translation;
    out << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
    out << YAML::Key << "Scale" << YAML::Value << tc.scale;

    out << YAML::EndMap;  // TransformComponent Map
  }

  if (scene.HasComponent<CameraComponent>(entity)) {
    out << YAML::Key << "CameraComponent";
    out << YAML::BeginMap;  // CameraComponent Map

    auto& cameraComponent = scene.GetComponent<CameraComponent>(entity);

    out << YAML::Key << "ProjectionType" << YAML::Value
        << (int)cameraComponent.projection_type;
    out << YAML::Key << "AspectRatio" << YAML::Value
        << cameraComponent.aspect_ratio;
    out << YAML::Key << "PerspectiveFOV" << YAML::Value
        << cameraComponent.pers_fov;
    out << YAML::Key << "PerspectiveNear" << YAML::Value
        << cameraComponent.pers_near;
    out << YAML::Key << "PerspectiveFar" << YAML::Value
        << cameraComponent.pers_far;
    out << YAML::Key << "OrthographicSize" << YAML::Value
        << cameraComponent.ortho_size;
    out << YAML::Key << "OrthographicNear" << YAML::Value
        << cameraComponent.ortho_near;
    out << YAML::Key << "OrthographicFar" << YAML::Value
        << cameraComponent.ortho_far;

    // out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
    out << YAML::Key << "FixedAspectRatio" << YAML::Value
        << cameraComponent.fixed_aspect_ratio;

    out << YAML::EndMap;  // CameraComponent Map
  }

  if (scene.HasComponent<SpriteRendererComponent>(entity)) {
    out << YAML::Key << "SpriteRendererComponent";
    out << YAML::BeginMap;  // SpriteRendererComponent Map

    auto& spriteRendererComponent =
        scene.GetComponent<SpriteRendererComponent>(entity);
    out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;

    out << YAML::EndMap;  // SpriteRendererComponent Map
  }

  out << YAML::EndMap;  // Entity Map
}

void SerializeScene(Scene& scene, const std::string& filepath) {
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "Scene" << YAML::Value << "Untitled";
  out << YAML::Key << "Entites" << YAML::Value << YAML::BeginSeq;
  scene.registry().each(
      [&](auto entity) { SerializeEntity(out, scene, entity); });
  out << YAML::EndSeq;
  out << YAML::EndMap;

  std::ofstream fout(filepath);
  fout << out.c_str();
}

bool DeserializeScene(Scene& scene, const std::string& filepath) {
  std::ifstream     fin(filepath);
  std::stringstream ss;
  ss << fin.rdbuf();

  YAML::Node data = YAML::Load(ss.str());
  if (!data["Scene"]) {
    return false;
  }

  std::string scene_name = data["Scene"].as<std::string>();
  SAMUI_ENGINE_TRACE("Deserializing scene '{0}'", scene_name);

  auto entities = data["Entites"];
  if (entities) {
    for (auto entity : entities) {
      uint64_t uuid = entity["Entity"].as<uint64_t>();  // TODO

      std::string name;
      auto        name_component = entity["NameComponent"];
      if (name_component) {
        name = name_component["Name"].as<std::string>();
      }

      SAMUI_ENGINE_TRACE("Deserializing entity with ID = {0}, name = {1}", uuid,
                         name);

      auto deserialized_entity = scene.CreateEntity(name);

      auto transformComponent = entity["TransformComponent"];
      if (transformComponent) {
        // Entities always have transforms
        auto& tc = scene.GetComponent<TransformComponent>(deserialized_entity);
        tc.translation = transformComponent["Translation"].as<glm::vec3>();
        tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
        tc.scale = transformComponent["Scale"].as<glm::vec3>();
      }

      auto cameraComponent = entity["CameraComponent"];
      if (cameraComponent) {
        auto& cc = scene.AddComponent<CameraComponent>(deserialized_entity);

        cc.projection_type =
            (CameraComponent::ProjectionType)cameraComponent["ProjectionType"]
                .as<int>();
        cc.aspect_ratio = cameraComponent["AspectRatio"].as<float>();

        cc.pers_fov = cameraComponent["PerspectiveFOV"].as<float>();
        cc.pers_near = cameraComponent["PerspectiveNear"].as<float>();
        cc.pers_far = cameraComponent["PerspectiveFar"].as<float>();

        cc.ortho_size = cameraComponent["OrthographicSize"].as<float>();
        cc.ortho_near = cameraComponent["OrthographicNear"].as<float>();
        cc.ortho_far = cameraComponent["OrthographicFar"].as<float>();

        cc.fixed_aspect_ratio = cameraComponent["FixedAspectRatio"].as<bool>();
      }

      auto spriteRendererComponent = entity["SpriteRendererComponent"];
      if (spriteRendererComponent) {
        auto& src =
            scene.AddComponent<SpriteRendererComponent>(deserialized_entity);
        src.color = spriteRendererComponent["Color"].as<glm::vec4>();
      }
    }
  }
}

}  // namespace Serialization

}  // namespace samui