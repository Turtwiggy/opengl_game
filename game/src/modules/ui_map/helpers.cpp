#include "helpers.hpp"

// components
#include "game_components.hpp"
#include "modules/editor_camera/components.hpp"
#include "modules/renderer/components.hpp"

// other libs
#include <nlohmann/json.hpp>

// std lib
#include <fstream>
#include <iostream>
#include <string>

namespace game2d {

class NJSONOutputArchive
{
public:
  NJSONOutputArchive() { root = nlohmann::json::array(); };

  // new element for serialization. giving you the amount of elements that is going to be
  // pumped into operator()(entt::entity ent) or operator()(entt::entity ent, const T &t)
  void operator()(std::underlying_type_t<entt::entity> size)
  {
    int a = 0;
    if (!current.empty()) {
      root.push_back(current);
    }
    current = nlohmann::json::array();
    current.push_back(size); // first element of each array keeps the amount of elements.
  }

  // persist entity ids
  void operator()(entt::entity entity)
  {
    // Here it is assumed that no custom entt-type is chosen
    current.push_back((uint32_t)entity);
  }

  // persist components
  template<typename T>
  void operator()(entt::entity ent, const T& t)
  {
    current.push_back((uint32_t)ent); // persist the entity id of the following component

    nlohmann::json json = t;
    current.push_back(json);
  }

  void close()
  {
    if (!current.empty()) {
      root.push_back(current);
    }
  }

  // create a json as string
  const std::string as_string()
  {
    std::string output = root.dump();
    return output;
  }

  // create bson-data
  const std::vector<uint8_t> as_bson()
  {
    std::vector<std::uint8_t> as_bson = nlohmann::json::to_bson(root);
    return as_bson;
  }

private:
  nlohmann::json root;
  nlohmann::json current;
};

class NJSONInputArchive
{
private:
  nlohmann::json root;
  nlohmann::json current;

  int root_idx = -1;
  int current_idx = 0;

public:
  NJSONInputArchive(const std::string& json_string) { root = nlohmann::json::parse(json_string); };

  ~NJSONInputArchive() {}

  void next_root()
  {
    root_idx++;
    if (root_idx >= root.size()) {
      // ERROR
      return;
    }
    current = root[root_idx];
    current_idx = 0;
  }

  void operator()(std::underlying_type_t<entt::entity>& s)
  {
    next_root();
    int size = current[0].get<int>();
    current_idx++;
    s = (std::underlying_type_t<entt::entity>)size; // pass amount to entt
  }

  void operator()(entt::entity& entity)
  {
    uint32_t ent = current[current_idx].get<uint32_t>();
    entity = entt::entity(ent);
    current_idx++;
  }

  template<typename T>
  void operator()(entt::entity& ent, T& t)
  {
    nlohmann::json component_data = current[current_idx * 2];

    auto comp = component_data.get<T>();
    t = comp;

    uint32_t _ent = current[current_idx * 2 - 1];
    ent = entt::entity(_ent); // last element is the entity-id
    current_idx++;
  }
};

} // namespace game2d

void
game2d::serialize_to_text(entt::registry& registry, std::string path)
{
  std::cout << "save entt registry to file: " << path << std::endl;

  // convert entt registry to string (or bson)

  NJSONOutputArchive json_archive;
  entt::basic_snapshot snapshot(registry);
  snapshot.entities(json_archive)
    .component<TagComponent, ColourComponent, PositionIntComponent, RenderSizeComponent, SpriteComponent>(json_archive);
  json_archive.close();

  std::string data = json_archive.as_string();

  // save to disk

  std::ofstream fout(path);
  fout << data.c_str();
}

void
game2d::deserialize_text_to_registry(entt::registry& registry, std::string path)
{
  std::cout << "load entt registry from file: " << path << std::endl;

  registry.each([&registry](auto entity) { registry.destroy(entity); });

  // load from disk

  std::ifstream t(path);
  std::stringstream buffer;
  buffer << t.rdbuf();
  const std::string data = buffer.str();

  // convert string (or bson) to entt registry

  auto& registry_to_load_in_to = registry;

  NJSONInputArchive json_in(data);
  entt::basic_snapshot_loader loader(registry_to_load_in_to);
  loader.entities(json_in)
    .component<TagComponent, ColourComponent, PositionIntComponent, RenderSizeComponent, SpriteComponent>(json_in);
};