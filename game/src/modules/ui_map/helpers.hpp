#pragma once

// other lib headers
#include <entt/entt.hpp>

// std libs
#include <vector>

namespace game2d {

void
serialize_to_text(entt::registry& registry, std::string path);

void
deserialize_text_to_registry(entt::registry& registry, std::string path);

std::vector<entt::entity>&
entities_at(int x, int y);

} // namespace game2d