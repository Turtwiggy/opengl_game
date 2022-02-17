#include "helpers.hpp"

// components
#include "modules/editor_camera/components.hpp"
#include "modules/renderer/components.hpp"

// other libs
#include <nlohmann/json.hpp>

namespace game2d {

void
serialize_entity(const entt::registry& registry, const entt::entity& eid)
{
  // out << YAML::BeginMap;
  // out << YAML::Key << "Entity" << YAML::Value << static_cast<uint32_t>(eid);

  // TODO: serialise entt::registry?
  // archive;
  // entt::basic_snapshot snapshot(registry);
  // snapshot.entities(archive).component<>(archive);
  // archive.close();
  // registry.

  // Serialize TagComponent..
  // const TagComponent& t = registry.get<TagComponent>(eid);
  // out << t;

  // rendering...

  // Serialize ColourComponent..
  // if (registry.all_of<ColourComponent>(eid)) {
  //   const ColourComponent& c = registry.get<ColourComponent>(eid);
  //   out << YAML::Key << "ColourComponent";
  //   out << YAML::BeginMap;
  //   out << YAML::Key << "Colour";
  //   out << YAML::Value << c.colour;
  //   out << YAML::EndMap;
  // }

  // // Serialize PositionIntComponent..
  // if (registry.all_of<PositionIntComponent>(eid)) {
  //   const PositionIntComponent& p = registry.get<PositionIntComponent>(eid);
  //   out << YAML::Key << "PositionIntComponent";
  //   out << YAML::BeginMap;
  //   out << YAML::Key << "PosX";
  //   out << YAML::Value << p.x;
  //   out << YAML::Key << "PosY";
  //   out << YAML::Value << p.y;
  //   out << YAML::Key << "PosDX";
  //   out << YAML::Value << p.dx;
  //   out << YAML::Key << "PosDY";
  //   out << YAML::Value << p.dy;
  //   out << YAML::EndMap;
  // }

  // // Serialize RenderSizeComponent..
  // if (registry.all_of<RenderSizeComponent>(eid)) {
  //   const RenderSizeComponent& r = registry.get<RenderSizeComponent>(eid);
  //   out << YAML::Key << "RenderSizeComponent";
  //   out << YAML::BeginMap;
  //   out << YAML::Key << "Width";
  //   out << YAML::Value << r.w;
  //   out << YAML::Key << "Height";
  //   out << YAML::Value << r.h;
  //   out << YAML::Key << "DHeight";
  //   out << YAML::Value << r.dh;
  //   out << YAML::Key << "DWidth";
  //   out << YAML::Value << r.dw;
  //   out << YAML::EndMap;
  // }

  // // Serialize SpriteComponent..
  // if (registry.all_of<SpriteComponent>(eid)) {
  //   const SpriteComponent& r = registry.get<SpriteComponent>(eid);
  //   out << YAML::Key << "SpriteComponent";
  //   out << YAML::BeginMap;
  //   out << YAML::Key << "Sprite";
  //   out << YAML::Value << static_cast<int>(r.sprite);
  //   out << YAML::EndMap;
  // }

  // out << YAML::EndMap; // Entity
}

} // namespace game2d

void
game2d::serialize_to_text(entt::registry& registry, std::string path)
{
  // entt::snapshot{ registry }

  // YAML::Emitter out;
  // out << YAML::BeginMap;
  // out << YAML::Key << "Scene";
  // out << YAML::Value << "Default Scene";
  // out << YAML::Key << "Entities";
  // out << YAML::Value;
  // out << YAML::BeginSeq;

  // Serialize all entities..
  // registry.each([&registry, &out](auto entity) {
  //   const auto& eid = entity;
  //   if (eid == entt::null)
  //     return;
  //   // .. except editor camera
  //   if (registry.all_of<EditorCameraComponent>(eid))
  //     return;
  //   serialize_entity(out, registry, eid);
  // });

  // out << YAML::EndSeq;
  // out << YAML::EndMap;

  // std::ofstream fout(path);
  // fout << out.c_str();
}

void
game2d::deserialize_text_to_registry(entt::registry& registry, std::string path){
  // std::cout << "load map from file: " << path << std::endl;

  // YAML::Node data;
  // try {
  //   data = YAML::LoadFile(path);
  // } catch (YAML::ParserException e) {
  //   std::cout << "scene could not load!" << std::endl;
  //   return;
  // }

  // auto scene = data["Scene"];
  // if (!scene)
  //   return;

  // // Delete all entities in the registry..
  // auto view = registry.view<TagComponent>();
  // view.each([&registry](const auto e, const auto& tag_component) {
  //   // .. except editor camera
  //   if (registry.all_of<EditorCameraComponent>(e))
  //     return;
  //   registry.destroy(e);
  // });

  // std::string scene_name = data["Scene"].as<std::string>();
  // std::cout << "Deserializing: " << scene_name << std::endl;

  // auto entities = data["Entities"];
  // if (entities) {
  //   for (auto entity : entities) {
  //     uint32_t id = entity["Entity"].as<uint32_t>();
  //     entt::entity r = registry.create();

  //     // TagComponent tag_component;
  //     // TagComponent::load(entity, tag_component);
  //     // registry.emplace<TagComponent>(r, tag_component);

  //     // load_component < load_component<ColourComponent>();
  //     // load_component<PositionIntComponent>();
  //     // load_component<RenderSizeComponent>();
  //     // load_component<SpriteComponent>();

  //     // ColourComponent colour_component;
  //     // ColourComponent::load(entity, colour_component);
  //     // registry.emplace<ColourComponent>(r, colour_component);

  //     // {
  //     //   const auto d = entity["ColourComponent"];
  //     //   if (d) {
  //     //     colour_component.colour = d["Colour"].as<glm::vec4>();
  //     //     registry.emplace<ColourComponent>(r, colour_component);
  //     //   }
  //     // }

  //     // PositionIntComponent position_int_component;
  //     // {
  //     //   const auto d = entity["PositionIntComponent"];
  //     //   if (d) {
  //     //     position_int_component.x = d["PosX"].as<int>();
  //     //     position_int_component.y = d["PosY"].as<int>();
  //     //     position_int_component.dx = d["PosDX"].as<float>();
  //     //     position_int_component.dy = d["PosDX"].as<float>();
  //     //     registry.emplace<PositionIntComponent>(r, position_int_component);
  //     //   }
  //     // }

  //     // RenderSizeComponent render_size_component;
  //     // {
  //     //   const auto d = entity["RenderSizeComponent"];
  //     //   if (d) {
  //     //     render_size_component.w = d["Width"].as<int>();
  //     //     render_size_component.h = d["Height"].as<int>();
  //     //     render_size_component.dw = d["DWidth"].as<float>();
  //     //     render_size_component.dh = d["DHeight"].as<float>();
  //     //     registry.emplace<RenderSizeComponent>(r, render_size_component);
  //     //   }
  //     // }

  //     // SpriteComponent sprite_component;
  //     // {
  //     //   const auto d = entity["SpriteComponent"];
  //     //   if (d) {
  //     //     auto sprite_int = d["Sprite"].as<int>();
  //     //     sprite_component.sprite = static_cast<sprite::type>(sprite_int);
  //     //     registry.emplace<SpriteComponent>(r, sprite_component);
  //     //   }
  //     // }

  //     //
  //     // game state...
  //     //
  //   }
};