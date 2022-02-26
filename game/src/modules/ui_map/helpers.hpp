#pragma once

// my libs
#include <modules/ui_map/components.hpp>

// other lib headers
#include <entt/entt.hpp>

// std libs
#include <vector>

namespace game2d {

void
serialize_to_text(entt::registry& registry, std::string path);

void
deserialize_text_to_registry(entt::registry& registry, std::string path);

// entity-map manipulation

std::vector<entt::entity>&
get_entities(SINGLETON_MapComponent& map, int x, int y);

void
add_entity_to_map(SINGLETON_MapComponent& map, const entt::entity& entity, int x, int y);

void
move_entity_on_map(SINGLETON_MapComponent& map, const entt::entity& entity, int old_x, int old_y, int new_x, int new_y);

} // namespace game2d