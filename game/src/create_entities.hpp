#pragma once

// other libs
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace game2d {

void
create_cursor(entt::registry& registry);

void
create_player(entt::registry& registry,
              int x,
              int y,
              const std::string& sprite,
              const glm::vec4& start_colour,
              const glm::vec4& highlight_colour);

void
create_debug_square(entt::registry& registry);

} // namespace game2d