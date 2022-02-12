#include "helpers.hpp"

// components
#include "editor_camera/components.hpp"
#include "renderer/components.hpp"

// other libs
#include <yaml-cpp/yaml.h>

// std libs
#include <fstream>
#include <iostream>

namespace YAML {

template<>
struct convert<glm::vec3>
{
  static Node encode(const glm::vec3& rhs)
  {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::vec3& rhs)
  {
    if (!node.IsSequence() || node.size() != 3)
      return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
  }
};

template<>
struct convert<glm::vec4>
{
  static Node encode(const glm::vec4& rhs)
  {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.push_back(rhs.w);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::vec4& rhs)
  {
    if (!node.IsSequence() || node.size() != 4)
      return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    rhs.w = node[3].as<float>();
    return true;
  }
};

} // namespace YAML

namespace game2d {

YAML::Emitter&
operator<<(YAML::Emitter& out, const glm::vec4& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
  return out;
}

void
serialize_entity(YAML::Emitter& out, const game2d::Coordinator& registry, const entt::entity& eid)
{
  out << YAML::BeginMap;
  out << YAML::Key << "Entity" << YAML::Value << static_cast<uint32_t>(eid);

  // TODO: serialise game2d::Coordinator?
  // archive;
  // entt::basic_snapshot snapshot(registry);
  // snapshot.entities(archive).component<>(archive);
  // archive.close();
  // registry.

  // Serialize TagComponent..
  if (registry.all_of<TagComponent>(eid)) {
    const TagComponent& t = registry.get<TagComponent>(eid);
    out << YAML::Key << "TagComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Tag";
    out << YAML::Value << t.tag;
    out << YAML::EndMap;
  }

  // rendering...

  // Serialize ColourComponent..
  if (registry.all_of<ColourComponent>(eid)) {
    const ColourComponent& c = registry.get<ColourComponent>(eid);
    out << YAML::Key << "ColourComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Colour";
    out << YAML::Value << c.colour;
    out << YAML::EndMap;
  }

  // Serialize PositionIntComponent..
  if (registry.all_of<PositionIntComponent>(eid)) {
    const PositionIntComponent& p = registry.get<PositionIntComponent>(eid);
    out << YAML::Key << "PositionIntComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "PosX";
    out << YAML::Value << p.x;
    out << YAML::Key << "PosY";
    out << YAML::Value << p.y;
    out << YAML::Key << "PosDX";
    out << YAML::Value << p.dx;
    out << YAML::Key << "PosDY";
    out << YAML::Value << p.dy;
    out << YAML::EndMap;
  }

  // Serialize RenderSizeComponent..
  if (registry.all_of<RenderSizeComponent>(eid)) {
    const RenderSizeComponent& r = registry.get<RenderSizeComponent>(eid);
    out << YAML::Key << "RenderSizeComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Width";
    out << YAML::Value << r.w;
    out << YAML::Key << "Height";
    out << YAML::Value << r.h;
    out << YAML::Key << "DHeight";
    out << YAML::Value << r.dh;
    out << YAML::Key << "DWidth";
    out << YAML::Value << r.dw;
    out << YAML::EndMap;
  }

  // Serialize SpriteComponent..
  if (registry.all_of<SpriteComponent>(eid)) {
    const SpriteComponent& r = registry.get<SpriteComponent>(eid);
    out << YAML::Key << "SpriteComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Sprite";
    out << YAML::Value << static_cast<int>(r.sprite);
    out << YAML::EndMap;
  }

  out << YAML::EndMap; // Entity
}

} // namespace game2d

void
game2d::serialize_to_text(game2d::Coordinator& registry, std::string path)
{
  // entt::snapshot{ registry }

  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "Scene";
  out << YAML::Value << "Default Scene";
  out << YAML::Key << "Entities";
  out << YAML::Value;
  out << YAML::BeginSeq;

  // Serialize all entities..
  registry.each([&registry, &out](auto entity) {
    const auto& eid = entity;
    if (eid == entt::null)
      return;
    // .. except editor camera
    if (registry.all_of<EditorCameraComponent>(eid))
      return;
    serialize_entity(out, registry, eid);
  });

  out << YAML::EndSeq;
  out << YAML::EndMap;

  std::ofstream fout(path);
  fout << out.c_str();
}

void
game2d::deserialize_text_to_registry(game2d::Coordinator& registry, std::string path)
{
  std::cout << "load map from file: " << path << std::endl;

  YAML::Node data;
  try {
    data = YAML::LoadFile(path);
  } catch (YAML::ParserException e) {
    std::cout << "scene could not load!" << std::endl;
    return;
  }

  auto scene = data["Scene"];
  if (!scene)
    return;

  // Delete all entities in the registry..
  auto view = registry.view<TagComponent>();
  view.each([&registry](const auto e, const auto& tag_component) {
    // .. except editor camera
    if (registry.all_of<EditorCameraComponent>(e))
      return;
    registry.destroy(e);
  });

  std::string scene_name = data["Scene"].as<std::string>();
  std::cout << "Deserializing: " << scene_name << std::endl;

  auto entities = data["Entities"];
  if (entities) {
    for (auto entity : entities) {
      uint32_t id = entity["Entity"].as<uint32_t>();
      entt::entity r = registry.create();

      std::string name = entity["TagComponent"]["Tag"].as<std::string>();
      registry.emplace<TagComponent>(r, name);

      //
      // rendering...
      //

      ColourComponent colour_component;
      {
        const auto d = entity["ColourComponent"];
        if (d) {
          colour_component.colour = d["Colour"].as<glm::vec4>();
          registry.emplace<ColourComponent>(r, colour_component);
        }
      }

      PositionIntComponent position_int_component;
      {
        const auto d = entity["PositionIntComponent"];
        if (d) {
          position_int_component.x = d["PosX"].as<int>();
          position_int_component.y = d["PosY"].as<int>();
          position_int_component.dx = d["PosDX"].as<float>();
          position_int_component.dy = d["PosDX"].as<float>();
          registry.emplace<PositionIntComponent>(r, position_int_component);
        }
      }

      RenderSizeComponent render_size_component;
      {
        const auto d = entity["RenderSizeComponent"];
        if (d) {
          render_size_component.w = d["Width"].as<int>();
          render_size_component.h = d["Height"].as<int>();
          render_size_component.dw = d["DWidth"].as<float>();
          render_size_component.dh = d["DHeight"].as<float>();
          registry.emplace<RenderSizeComponent>(r, render_size_component);
        }
      }

      SpriteComponent sprite_component;
      {
        const auto d = entity["SpriteComponent"];
        if (d) {
          auto sprite_int = d["Sprite"].as<int>();
          sprite_component.sprite = static_cast<sprite::type>(sprite_int);
          registry.emplace<SpriteComponent>(r, sprite_component);
        }
      }
    }
  }
};