#include "helpers.hpp"

// components
#include "gameplay_components/components.hpp"
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

}; // namespace game2d

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

void
game2d::remove_entity_from_map(SINGLETON_MapComponent& map, const entt::entity& entity)
{
  for (auto& eids : map.entities) {
    auto it = eids.begin();
    while (it != eids.end()) {
      const entt::entity& e = *it;
      if (e == entity) {
        it = eids.erase(it);
      } else {
        ++it;
      }
    }
  }
};

bool
game2d::remove_entity_from_map_at_location(SINGLETON_MapComponent& map, const entt::entity& entity, int x, int y)
{
  auto& entities = get_entities(map, x, y);
  bool removed = false;

  auto it = entities.begin();
  while (it != entities.end()) {
    const entt::entity& e = *it;
    if (e == entity) {
      it = entities.erase(it); // erase old entity in old grid slot
      removed = true;
    } else {
      ++it;
    }
  }

  return removed;
};

std::vector<entt::entity>&
game2d::get_entities(SINGLETON_MapComponent& map, int x, int y)
{
  return map.entities[map.size_x * y + x];
};

void
game2d::add_entity_to_map(SINGLETON_MapComponent& map, const entt::entity& entity, int x, int y)
{
  map.entities[map.size_x * y + x].push_back(entity);
};

void
game2d::move_entity_on_map(SINGLETON_MapComponent& map,
                           const entt::entity& entity,
                           int old_x,
                           int old_y,
                           int new_x,
                           int new_y)
{
  bool removed = remove_entity_from_map_at_location(map, entity, old_x, old_y);
  if (removed) {
    add_entity_to_map(map, entity, new_x, new_y);
  }
};

bool
game2d::grid_cell_contains_tile_type(const entt::registry& registry,
                                     SINGLETON_MapComponent& map,
                                     const int x,
                                     const int y,
                                     const TileType& tile_type)
{
  const auto& entities = get_entities(map, x, y);
  for (const auto& entity : entities) {
    if (registry.any_of<GridTypeComponent>(entity)) {
      const auto& gtc = registry.get<GridTypeComponent>(entity);
      return gtc.type == tile_type;
    }
  }
  return false;
};

void
game2d::update_grid_entity(entt::registry& registry, entt::entity& entity, TileType& type, sprite::type& sprite_type)
{
  if (registry.any_of<GridTypeComponent>(entity)) {
    auto& gtc = registry.get<GridTypeComponent>(entity);
    gtc.type = type;
  }
  if (registry.any_of<SpriteComponent>(entity)) {
    auto& s = registry.get<SpriteComponent>(entity);
    s.sprite = sprite_type;
  }
};
