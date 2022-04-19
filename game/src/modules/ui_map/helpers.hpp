#pragma once

// my libs
#include "gameplay_components/components.hpp"
#include "gameplay_helpers/tile_types.hpp"
#include "modules/ui_map/components.hpp"

// other lib headers
#include <entt/entt.hpp>

// std libs
#include <vector>

namespace game2d {

struct GridTypeComponent
{
  TileType type = TileType::FLOOR;
};

void
serialize_to_text(entt::registry& registry, std::string path);

void
deserialize_text_to_registry(entt::registry& registry, std::string path);

// entity-map manipulation

// C
void
add_entity_to_map(SINGLETON_MapComponent& map, const entt::entity& entity, int x, int y);

// R
std::vector<entt::entity>&
get_entities(SINGLETON_MapComponent& map, int x, int y);

// U
void
move_entity_on_map(SINGLETON_MapComponent& map, const entt::entity& entity, int old_x, int old_y, int new_x, int new_y);

// D
void
remove_entity_from_map(SINGLETON_MapComponent& map, const entt::entity& entity);

bool
remove_entity_from_map_at_location(SINGLETON_MapComponent& map, const entt::entity& entity, int x, int y);

// entity-map inner-cell manupulations

bool
grid_cell_contains_tile_type(const entt::registry& registry,
                             SINGLETON_MapComponent& map,
                             const int x,
                             const int y,
                             const TileType& tile_type);

void
update_grid_entity(entt::registry& registry, entt::entity& entity, TileType& type, sprite::type& sprite_type);

} // namespace game2d